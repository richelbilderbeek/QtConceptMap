#include "qtconceptmapcommandselect.h"

#include <cassert>
#include <boost/algorithm/string/trim.hpp>
#include <gsl/gsl_assert>
#include "count_vertices_with_selectedness.h"
#include "container.h"
#include "conceptmap.h"
#include "conceptmaphelper.h"
#include "conceptmapnode.h"
#include "qtconceptmap.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmaphelper.h"
#include "qtconceptmapcommandselectedge.h"
#include "qtconceptmapcommandselectnode.h"

ribi::cmap::CommandSelect::CommandSelect(
  QtConceptMap& qtconceptmap,
  QGraphicsItem& item
)
  : Command(qtconceptmap),
    m_cmd{nullptr}
{
  if (QtEdge* const qtedge = dynamic_cast<QtEdge*>(&item))
  {
    m_cmd = new CommandSelectEdge(qtconceptmap, qtedge, this);
  }
  else if (QtNode* const qtnode = dynamic_cast<QtNode*>(&item))
  {
    if (QtEdge * const qtedge2 = FindQtEdge(qtnode, GetQtConceptMap()))
    {
      m_cmd = new CommandSelectEdge(qtconceptmap, qtedge2, this);
    }
    else
    {
      m_cmd = new CommandSelectNode(qtconceptmap, qtnode, this);
    }
  }
  if (!m_cmd)
  {
    throw std::invalid_argument("item is not a QtEdge nor QtNode");
  }

  //QCommands have a text
  {
    std::stringstream msg;
    msg << "Select item";
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
  for (QGraphicsItem * const item: qtconceptmap.items())
  {
    if (QtEdge * const qtedge = dynamic_cast<QtEdge*>(item))
    {
      if (GetText(*qtedge) == t)
      {
        return new CommandSelect(qtconceptmap, *qtedge);
      }
    }
    if (QtNode * const qtnode = dynamic_cast<QtNode*>(item))
    {
      if (GetText(*qtnode) == t)
      {
        return new CommandSelect(qtconceptmap, *qtnode);
      }
    }
  }
  return nullptr;
}

void ribi::cmap::CommandSelect::Redo()
{
  #ifndef NDEBUG
  const int n_selected_qtedges_before = CountSelectedQtEdges(GetQtConceptMap());
  const int n_selected_qtnodes_before = CountSelectedQtNodes(GetQtConceptMap());
  const int n_selected_items_before = n_selected_qtedges_before + n_selected_qtnodes_before;
  #endif

  m_cmd->redo();
  /*
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
  */

  #ifndef NDEBUG
  const int n_selected_qtedges_after = CountSelectedQtEdges(GetQtConceptMap());
  const int n_selected_qtnodes_after = CountSelectedQtNodes(GetQtConceptMap());
  const int n_selected_items_after = n_selected_qtedges_after + n_selected_qtnodes_after;
  assert(n_selected_items_after > n_selected_items_before);
  #endif

  
}

void ribi::cmap::CommandSelect::Undo()
{
  m_cmd->undo();
  /*
  if (m_renamed_qtedge)
  {
    SetSelectedness(false, *m_renamed_qtedge, GetQtConceptMap());
  }
  if (m_renamed_qtnode)
  {
    SetSelectedness(false, *m_renamed_qtnode, GetQtConceptMap());
  }
  */
}
