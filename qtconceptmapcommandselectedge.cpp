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

void ribi::cmap::CommandSelectEdge::redo()
{
  #ifndef NDEBUG
  const int n_selected_qtedges_before = CountSelectedQtEdges(GetQtConceptMap());
  const int n_selected_qtnodes_before = CountSelectedQtNodes(GetQtConceptMap());
  const int n_selected_items_before = n_selected_qtedges_before + n_selected_qtnodes_before;
  #endif

  assert(m_qtedge);
  if (HasExamples(*m_qtedge))
  {
    SetQtExamplesBuddy(GetQtConceptMap(), m_qtedge);
  }
  SetQtToolItemBuddy(GetQtConceptMap(), m_qtedge);
  SetSelectedness(true, *m_qtedge, GetQtConceptMap());
  assert(CountSelectedQtEdges(GetQtConceptMap()) > 0 || IsConnectedToCenterNode(*m_qtedge));

  #ifndef NDEBUG
  const int n_selected_qtedges_after = CountSelectedQtEdges(GetQtConceptMap());
  const int n_selected_qtnodes_after = CountSelectedQtNodes(GetQtConceptMap());
  const int n_selected_items_after = n_selected_qtedges_after + n_selected_qtnodes_after;
  assert(n_selected_items_after > n_selected_items_before);
  #endif

  CheckInvariants(GetQtConceptMap());
}

void ribi::cmap::CommandSelectEdge::undo()
{
  SetSelectedness(false, *m_qtedge, GetQtConceptMap());
  CheckInvariants(GetQtConceptMap());
}
