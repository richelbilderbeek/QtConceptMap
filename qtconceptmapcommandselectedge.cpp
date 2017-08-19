#include "qtconceptmapcommandselectedge.h"

#include <cassert>
#include <boost/graph/isomorphism.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/trim_all.hpp>
#include <boost/lexical_cast.hpp>
#include <gsl/gsl_assert>
#include <QApplication>
#include "count_vertices_with_selectedness.h"
#include "container.h"
#include "conceptmap.h"
#include "conceptmaphelper.h"
#include "conceptmapnode.h"
#include "qtconceptmap.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmaphelper.h"

ribi::cmap::CommandSelectEdge::CommandSelectEdge(
  QtConceptMap& qtconceptmap,
  QtEdge * const qtedge
)
  : Command(qtconceptmap),
    m_qtedge{qtedge}
{

  if (!m_qtedge)
  {
    throw std::invalid_argument("Cannot select a nullptr QtEdge");
  }
  if (IsSelected(*m_qtedge))
  {
    throw std::invalid_argument("Cannot select a QtEdge that is already selected");
  }

  //QCommands have a text
  {
    std::stringstream msg;
    msg << "Select edge";
    this->setText(msg.str().c_str());
  }
}

ribi::cmap::CommandSelectEdge * ribi::cmap::ParseCommandSelectEdge(
  QtConceptMap& qtconceptmap, std::string s)
{
  //"select(my text)"
  boost::algorithm::trim(s);
  const std::string str_begin = "select_edge(";
  if (s.substr(0, str_begin.size()) != str_begin) return nullptr;
  if (s.back() != ')') return nullptr;
  //"my text"
  const std::string t = s.substr(str_begin.size(), s.size() - str_begin.size() - 1);
  assert(t[0] != '(');
  assert(t.back() != ')');
  QtEdge * const first_qtedge = FindFirstQtEdge(
    qtconceptmap,
    [t](const QtEdge* qtedge) { return GetText(*qtedge) == t; }
  );
  return new CommandSelectEdge(qtconceptmap, first_qtedge);
}

void ribi::cmap::CommandSelectEdge::Redo()
{
  #ifndef NDEBUG
  const int n_selected_qtedges_before = CountSelectedQtEdges(GetQtConceptMap());
  #endif

  m_prev_qttoolitem_buddy = GetQtToolItemBuddy(GetQtConceptMap());

  assert(m_qtedge);

  Select(GetQtConceptMap(), *m_qtedge);

  #ifndef NDEBUG
  const int n_selected_qtedges_after = CountSelectedQtEdges(GetQtConceptMap());
  Ensures(n_selected_qtedges_after == n_selected_qtedges_before + 1);
  #endif
}

void ribi::cmap::CommandSelectEdge::Undo()
{
  if (m_prev_qttoolitem_buddy && HasExamples(*m_prev_qttoolitem_buddy))
  {
    SetQtExamplesBuddy(GetQtConceptMap(), m_prev_qttoolitem_buddy);
  }
  else
  {
    QtNode * const no_qtnode{nullptr};
    SetQtExamplesBuddy(GetQtConceptMap(), no_qtnode);
  }

  SetQtToolItemBuddy(GetQtConceptMap(), m_prev_qttoolitem_buddy);

  SetSelectedness(false, *m_qtedge, GetQtConceptMap());
}
