#include "qtconceptmapcommandselectnode.h"

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

ribi::cmap::CommandSelectNode::CommandSelectNode(
  QtConceptMap& qtconceptmap,
  QtNode * const qtnode
)
  : Command(qtconceptmap),
    m_prev_qtexamplesitem_buddy{nullptr},
    m_prev_qttoolitem_buddy{nullptr},
    m_qtnode{qtnode}
{
  if (!m_qtnode)
  {
    throw std::invalid_argument("Cannot select nullptr QtNode");
  }
  if (IsQtNodeOnEdge(qtnode, GetQtConceptMap()))
  {
    throw std::invalid_argument(
      "Cannot select QtNode on QtEdge, "
      "use CommandSelectEdge instead");
  }
  if (m_qtnode->isSelected())
  {
    throw std::invalid_argument("Cannot select QtNode that is already selected");
  }
  //QCommands have a text
  {
    std::stringstream msg;
    msg << "Select node";
    this->setText(msg.str().c_str());
  }
}

ribi::cmap::CommandSelectNode * ribi::cmap::ParseCommandSelectNode(
  QtConceptMap& qtconceptmap, std::string s)
{
  //"select(my text)"
  boost::algorithm::trim(s);
  const std::string str_begin = "select_node(";
  if (s.substr(0, str_begin.size()) != str_begin) return nullptr;
  if (s.back() != ')') return nullptr;
  //"my text"
  const std::string t = s.substr(str_begin.size(), s.size() - str_begin.size() - 1);
  assert(t[0] != '(');
  assert(t.back() != ')');
  QtNode * const first_qtnode = FindFirstQtNode(
    qtconceptmap,
    [&qtconceptmap, t](QtNode * const qtnode)
    {
      return GetText(*qtnode) == t
        && !IsQtNodeOnEdge(qtnode, qtconceptmap);
    }
  );
  return new CommandSelectNode(qtconceptmap, first_qtnode);
}

void ribi::cmap::CommandSelectNode::redo()
{
  #ifndef NDEBUG
  const int n_selected_qtedges_before = CountSelectedQtEdges(GetQtConceptMap());
  const int n_selected_qtnodes_before = CountSelectedQtNodes(GetQtConceptMap());
  const int n_selected_items_before = n_selected_qtedges_before + n_selected_qtnodes_before;
  #endif

  m_prev_qtexamplesitem_buddy = GetQtExamplesItemBuddy(GetQtConceptMap());
  m_prev_qttoolitem_buddy = GetQtToolItemBuddy(GetQtConceptMap());


  assert(m_qtnode);
  if (HasExamples(*m_qtnode))
  {
    SetQtExamplesBuddy(GetQtConceptMap(), m_qtnode);
  }
  SetQtToolItemBuddy(GetQtConceptMap(), m_qtnode);
  SetSelectedness(true, *m_qtnode, GetQtConceptMap());

  #ifndef NDEBUG
  const int n_selected_qtedges_after = CountSelectedQtEdges(GetQtConceptMap());
  const int n_selected_qtnodes_after = CountSelectedQtNodes(GetQtConceptMap());
  const int n_selected_items_after = n_selected_qtedges_after + n_selected_qtnodes_after;
  //May also select nodes that are already selected
  assert(n_selected_items_after > n_selected_items_before);
  #endif

  CheckInvariants(GetQtConceptMap());
}

void ribi::cmap::CommandSelectNode::undo()
{
  CheckInvariants(GetQtConceptMap());

  if (const QtNode * const qtnode = dynamic_cast<const QtNode*>(m_prev_qtexamplesitem_buddy))
  {
    SetQtExamplesBuddy(GetQtConceptMap(), qtnode);
  }
  else if (const QtEdge * const qtedge = dynamic_cast<const QtEdge*>(m_prev_qtexamplesitem_buddy))
  {
    SetQtExamplesBuddy(GetQtConceptMap(), qtedge);
  }
  else
  {
    const QtNode * const no_node{nullptr};
    SetQtExamplesBuddy(GetQtConceptMap(), no_node);
  }

  SetQtToolItemBuddy(GetQtConceptMap(), m_prev_qttoolitem_buddy);

  SetSelectedness(false, *m_qtnode, GetQtConceptMap());
  CheckInvariants(GetQtConceptMap());
}
