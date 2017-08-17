#include "qtconceptmapcommandunselectnode.h"

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

ribi::cmap::CommandUnselectNode::CommandUnselectNode(
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
    throw std::invalid_argument("Cannot unselect nullptr QtNode");
  }
  if (IsQtNodeOnEdge(qtnode, GetQtConceptMap()))
  {
    throw std::invalid_argument(
      "Cannot unselect QtNode on QtEdge, "
      "use CommandUnselectEdge instead");
  }
  if (!m_qtnode->isSelected())
  {
    throw std::invalid_argument("Cannot unselect QtNode that is already unselected");
  }
  //QCommands have a text
  {
    std::stringstream msg;
    msg << "Unselect node";
    this->setText(msg.str().c_str());
  }
}

ribi::cmap::CommandUnselectNode * ribi::cmap::ParseCommandUnselectNode(
  QtConceptMap& qtconceptmap, std::string s)
{
  //"unselect(my text)"
  boost::algorithm::trim(s);
  const std::string str_begin = "unselect_node(";
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
  return new CommandUnselectNode(qtconceptmap, first_qtnode);
}

void ribi::cmap::CommandUnselectNode::redo()
{
  #ifndef NDEBUG
  const int n_selected_qtedges_before = CountSelectedQtEdges(GetQtConceptMap());
  const int n_selected_qtnodes_before = CountSelectedQtNodes(GetQtConceptMap());
  const int n_selected_items_before = n_selected_qtedges_before + n_selected_qtnodes_before;
  #endif

  m_prev_qtexamplesitem_buddy = GetQtExamplesItemBuddy(GetQtConceptMap());
  m_prev_qttoolitem_buddy = GetQtToolItemBuddy(GetQtConceptMap());


  assert(m_qtnode);
  QtNode * const no_qtnode{nullptr};
  if (HasExamples(*m_qtnode))
  {
    SetQtExamplesBuddy(GetQtConceptMap(), no_qtnode);
  }
  SetQtToolItemBuddy(GetQtConceptMap(), no_qtnode);
  SetSelectedness(false, *m_qtnode, GetQtConceptMap());

  {
    const auto qtnodes = ribi::cmap::GetSelectedQtNodesAlsoOnQtEdge(GetScene(*this));
    if (qtnodes.size() == 1)
    {
      SetQtToolItemBuddy(GetQtConceptMap(), qtnodes[0]);
      if (HasExamples(*qtnodes[0]))
      {
        SetQtExamplesBuddy(GetQtConceptMap(), qtnodes[0]);
      }
    }
  }

  #ifndef NDEBUG
  const int n_selected_qtedges_after = CountSelectedQtEdges(GetQtConceptMap());
  const int n_selected_qtnodes_after = CountSelectedQtNodes(GetQtConceptMap());
  const int n_selected_items_after = n_selected_qtedges_after + n_selected_qtnodes_after;
  assert(n_selected_items_after < n_selected_items_before);
  #endif

  CheckInvariants(GetQtConceptMap());
}

void ribi::cmap::CommandUnselectNode::undo()
{
  CheckInvariants(GetQtConceptMap());

  if (const QtNode * const qtnode = dynamic_cast<const QtNode*>(m_prev_qtexamplesitem_buddy))
  {
    if (HasExamples(*qtnode))
    {
      SetQtExamplesBuddy(GetQtConceptMap(), qtnode);
    }
  }
  else if (const QtEdge * const qtedge = dynamic_cast<const QtEdge*>(m_prev_qtexamplesitem_buddy))
  {
    assert(HasExamples(*qtedge)); //Should fail one day
    SetQtExamplesBuddy(GetQtConceptMap(), qtedge);
  }
  else
  {
    const QtNode * const no_node{nullptr};
    SetQtExamplesBuddy(GetQtConceptMap(), no_node);
  }

  SetQtToolItemBuddy(GetQtConceptMap(), m_prev_qttoolitem_buddy);

  SetSelectedness(true, *m_qtnode, GetQtConceptMap());
  CheckInvariants(GetQtConceptMap());
}