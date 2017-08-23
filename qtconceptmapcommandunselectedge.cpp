#include "qtconceptmapcommandunselectedge.h"

#include <cassert>
#include <boost/algorithm/string/trim.hpp>
#include <gsl/gsl_assert>
//#include "count_vertices_with_selectedness.h"
//#include "container.h"
//#include "conceptmap.h"
//#include "conceptmaphelper.h"
//#include "conceptmapnode.h"
#include "qtconceptmap.h"
#include "qtconceptmapqtedge.h"
//#include "qtconceptmaphelper.h"

ribi::cmap::CommandUnselectEdge::CommandUnselectEdge(
  QtConceptMap& qtconceptmap,
  QtEdge * const qtedge
)
  : Command(qtconceptmap),
    m_prev_qttoolitem_buddy{nullptr},
    m_qtedge{qtedge}
{
  if (!m_qtedge)
  {
    throw std::invalid_argument("Cannot unselect nullptr QtEdge");
  }
  if (!IsSelected(*m_qtedge))
  {
    throw std::invalid_argument("Cannot unselect QtEdge that is already unselected");
  }
  //QCommands have a text
  {
    std::stringstream msg;
    msg << "Unselect edge";
    this->setText(msg.str().c_str());
  }
}

ribi::cmap::CommandUnselectEdge * ribi::cmap::ParseCommandUnselectEdge(
  QtConceptMap& qtconceptmap, std::string s)
{
  //"unselect(my text)"
  boost::algorithm::trim(s);
  const std::string str_begin = "unselect_edge(";
  if (s.substr(0, str_begin.size()) != str_begin) return nullptr;
  if (s.back() != ')') return nullptr;
  //"my text"
  const std::string t = s.substr(str_begin.size(), s.size() - str_begin.size() - 1);
  assert(t[0] != '(');
  assert(t.back() != ')');
  QtEdge * const first_qtedge = FindFirstQtEdge(qtconceptmap, QtEdgeHasName(t));
  return new CommandUnselectEdge(qtconceptmap, first_qtedge);
}

void ribi::cmap::CommandUnselectEdge::Redo()
{
  #ifndef NDEBUG
  const int n_selected_qtedges_before = CountSelectedQtEdges(GetQtConceptMap());
  #endif

  m_prev_qttoolitem_buddy = GetQtToolItemBuddy(GetQtConceptMap());

  assert(m_qtedge);
  Unselect(GetQtConceptMap(), *m_qtedge);

  #ifndef NDEBUG
  const int n_selected_qtedges_after = CountSelectedQtEdges(GetQtConceptMap());
  Ensures(n_selected_qtedges_after < n_selected_qtedges_before);
  #endif
}

void ribi::cmap::CommandUnselectEdge::Undo()
{
  if (m_prev_qttoolitem_buddy && HasExamples(*m_prev_qttoolitem_buddy))
  {
    SetQtExamplesBuddy(GetQtConceptMap(), m_prev_qttoolitem_buddy);
  }
  else
  {
    const QtEdge * const no_qtedge{nullptr};
    SetQtExamplesBuddy(GetQtConceptMap(), no_qtedge);
  }
  SetQtToolItemBuddy(GetQtConceptMap(), m_prev_qttoolitem_buddy);
  SetSelectedness(true, *m_qtedge, GetQtConceptMap());

}
