#include "qtconceptmapcommandselect.h"

#include <cassert>
//#include <boost/graph/isomorphism.hpp>
#include <boost/algorithm/string/trim.hpp>
//#include <boost/algorithm/string/trim_all.hpp>
//#include <boost/lexical_cast.hpp>
#include <gsl/gsl_assert>
//#include <QApplication>
#include "count_vertices_with_selectedness.h"
#include "container.h"
#include "conceptmap.h"
#include "conceptmaphelper.h"
#include "conceptmapnode.h"
#include "qtconceptmap.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmaphelper.h"

ribi::cmap::CommandSelect::CommandSelect(
  QtConceptMap& qtconceptmap,
  const std::string& name
)
  : Command(qtconceptmap),
    m_renamed_qtedge{nullptr},
    m_renamed_qtnode{nullptr},
    m_name{name}
{
  //QCommands have a text
  {
    std::stringstream msg;
    msg << "Select node or edge with name '"
      << m_name << "'";
    this->setText(msg.str().c_str());
  }
}

ribi::cmap::CommandSelect * ribi::cmap::ParseCommandSelect(
  QtConceptMap& qtconceptmap, std::string s)
{
  //"select(my text)"
  boost::algorithm::trim(s);
  const std::string str_begin = "select(";
  if (s.substr(0, str_begin.size()) != str_begin) return nullptr;
  if (s.back() != ')') return nullptr;
  //"my text"
  const std::string t = s.substr(str_begin.size(), s.size() - str_begin.size() - 1);
  assert(t[0] != '(');
  assert(t.back() != ')');
  return new CommandSelect(qtconceptmap, t);
}

void ribi::cmap::CommandSelect::Redo()
{
  #ifndef NDEBUG
  const int n_selected_qtedges_before = CountSelectedQtEdges(GetQtConceptMap());
  const int n_selected_qtnodes_before = CountSelectedQtNodes(GetQtConceptMap());
  const int n_selected_items_before = n_selected_qtedges_before + n_selected_qtnodes_before;
  #endif

  m_renamed_qtnode = FindFirstQtNode(GetQtConceptMap(),
    [name = m_name, &qtconceptmap = GetQtConceptMap()](QtNode * const qtnode)
    {
      return name == GetText(*qtnode) && !IsQtNodeOnEdge(qtnode, qtconceptmap);
    }
  );
  if (m_renamed_qtnode)
  {
    m_renamed_qtedge = nullptr;
    if (HasExamples(*m_renamed_qtnode))
    {
      SetQtExamplesBuddy(GetQtConceptMap(), m_renamed_qtnode);
    }
    SetQtToolItemBuddy(GetQtConceptMap(), m_renamed_qtnode);
    SetSelectedness(true, *m_renamed_qtnode, GetQtConceptMap());
  }
  else
  {
    assert(!m_renamed_qtnode);
    m_renamed_qtedge = FindFirstQtEdge(GetQtConceptMap().GetScene(),
      [name = m_name](QtEdge * const qtedge)
      {
        return name == GetText(*qtedge);
      }
    );
    if (m_renamed_qtedge)
    {
      if (HasExamples(*m_renamed_qtedge))
      {
        SetQtExamplesBuddy(GetQtConceptMap(), m_renamed_qtedge);
      }
      SetQtToolItemBuddy(GetQtConceptMap(), m_renamed_qtedge);
      SetSelectedness(true, *m_renamed_qtedge, GetQtConceptMap());
      assert(CountSelectedQtEdges(GetQtConceptMap()) > 0 || IsConnectedToCenterNode(*m_renamed_qtedge));
    }
  }
  if (!m_renamed_qtedge && !m_renamed_qtnode)
  {
    std::stringstream msg;
    msg << "Could not find a QtEdge nor QtNode with text '"
      << m_name << "'";
    throw std::invalid_argument(msg.str());
  }
  assert((m_renamed_qtedge != nullptr) ^ (m_renamed_qtnode != nullptr));

  #ifndef NDEBUG
  const int n_selected_qtedges_after = CountSelectedQtEdges(GetQtConceptMap());
  const int n_selected_qtnodes_after = CountSelectedQtNodes(GetQtConceptMap());
  const int n_selected_items_after = n_selected_qtedges_after + n_selected_qtnodes_after;
  assert(n_selected_items_after > n_selected_items_before);
  #endif

  
}

void ribi::cmap::CommandSelect::Undo()
{
  if (m_renamed_qtedge)
  {
    SetSelectedness(false, *m_renamed_qtedge, GetQtConceptMap());
  }
  if (m_renamed_qtnode)
  {
    SetSelectedness(false, *m_renamed_qtnode, GetQtConceptMap());
  }
  
}
