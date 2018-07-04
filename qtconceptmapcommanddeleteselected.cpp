#include "qtconceptmapcommanddeleteselected.h"

#include <cassert>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <QDebug>
#include "conceptmap.h"
#include "conceptmapedge.h"
#include "conceptmapnode.h"
#include "conceptmapconcept.h"
#include "qtconceptmap.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmaphelper.h"
#include "find_first_custom_edge_with_my_edge.h"
#include "find_first_custom_vertex_with_my_vertex.h"
#include "remove_selected_custom_edges_and_vertices.h"
#include "qtconceptmapexamplesitem.h"
#include "qtconceptmaptoolsitem.h"

ribi::cmap::CommandDeleteSelected::CommandDeleteSelected(
  QtConceptMap& qtconceptmap
)
  : Command(qtconceptmap),
    m_focus_item_before{qtconceptmap.GetScene().focusItem()},
    m_qtedges_removed{},
    m_qtnodes_removed{},
    m_selected_before{qtconceptmap.GetScene().selectedItems()},
    m_tool_item_old_buddy{qtconceptmap.GetQtToolItem().GetBuddyItem()}
{
  setText("delete selected nodes and edges");
}


void ribi::cmap::CommandDeleteSelected::AddDeletedQtEdges()
{
  for (const auto qtedge: m_qtedges_removed)
  {
    assert(qtedge);
    AddQtEdge(qtedge, GetQtConceptMap());
  }
}

void ribi::cmap::CommandDeleteSelected::AddDeletedQtNodes()
{
  for (const auto qtnode: m_qtnodes_removed)
  {
    assert(qtnode);
    AddQtNode(qtnode, GetQtConceptMap());
  }
}

bool ribi::cmap::CommandDeleteSelected::AllHaveSameScene() const noexcept
{
  std::set<QGraphicsScene*> s;
  std::transform(
    std::begin(m_qtedges_removed),
    std::end(m_qtedges_removed),
    std::inserter(s, s.begin()),
    [](const QtEdge * const qtedge) { return qtedge->scene(); }
  );
  std::transform(
    std::begin(m_qtnodes_removed),
    std::end(m_qtnodes_removed),
    std::inserter(s, s.begin()),
    [](const QtNode * const qtnode) { return qtnode->scene(); }
  );
  return s.empty() || s.size() == 1;
}


void ribi::cmap::CommandDeleteSelected::CheckQtNodesBeforeDelete() const noexcept
{
  for (const auto qtnode: m_qtnodes_removed)
  {
    assert(qtnode->scene());
    assert(!qtnode->parentItem()); //Just to measure
    assert(!qtnode->parentObject()); //Just to measure
    assert(!qtnode->parentWidget()); //Just to measure
  }
}

void ribi::cmap::CommandDeleteSelected::RemoveSelectedQtEdges()
{
  m_qtedges_removed = GetSelectedQtEdges(GetQtConceptMap());
  for (QtEdge * const qtedge: GetSelectedQtEdges(GetQtConceptMap()))
  {
    SetSelectedness(false, *qtedge, GetQtConceptMap());
    GetQtConceptMap().GetScene().removeItem(qtedge);
    assert(!qtedge->scene());
  }
}

void ribi::cmap::CommandDeleteSelected::RemoveSelectedQtNodes()
{
  m_qtnodes_removed = GetSelectedQtNodes(GetQtConceptMap());
  for (QtNode * const qtnode: GetSelectedQtNodes(GetQtConceptMap()))
  {
    SetSelectedness(false, *qtnode, GetQtConceptMap());
    GetScene(*this).removeItem(qtnode);
    assert(!qtnode->scene());
  }
}

void ribi::cmap::CommandDeleteSelected::Redo()
{
  assert(AllHaveSameScene());

  if (IsQtCenterNodeSelected(GetQtConceptMap()))
  {
    throw std::invalid_argument("Will not delete center node");
  }

  //Count the number of vertices and edges selected
  {
    const int n_vertices_selected{CountSelectedQtNodes(GetQtConceptMap())};
    const int n_edges_selected{CountSelectedQtEdges(GetQtConceptMap())};
    if (n_vertices_selected + n_edges_selected == 0) {
      std::stringstream msg;
      msg << __func__ << ": no edges nor vertices selected, cannot delete zero selected items";
      throw std::logic_error(msg.str());
    }
  }

  assert(AllHaveSameScene());
  GetScene(*this).clearFocus();
  GetQtToolItem(*this).SetBuddyItem(nullptr);

  //Each QtEdge that is on a QtNode that is selected to be deleted,
  //must be selected, so it will be deleted as well
  SelectAllQtEdgesOnSelectedQtNodes();

  RemoveSelectedQtEdges();
  RemoveSelectedQtNodes();

  assert(AllHaveSameScene());
}

void ribi::cmap::CommandDeleteSelected::SelectAllQtEdgesOnSelectedQtNodes()
{
  for (QtEdge * qtedge: GetQtEdges(GetQtConceptMap()))
  {
    if (qtedge->GetFrom()->isSelected())
    {
      SetSelectedness(true, *qtedge, GetQtConceptMap());
    }
    else if (qtedge->GetTo()->isSelected())
    {
      SetSelectedness(true, *qtedge, GetQtConceptMap());
    }
  }
}

void ribi::cmap::CommandDeleteSelected::SetSelected(
  const QList<QGraphicsItem *>& v,
  const bool is_selected
) noexcept
{
  for (auto item: v)
  {
    if (QtNode* const qtnode = dynamic_cast<QtNode*>(item))
    {
      SetSelectedness(is_selected, *qtnode, GetQtConceptMap());
    }
    else if (QtEdge* const qtedge = dynamic_cast<QtEdge*>(item))
    {
      SetSelectedness(is_selected, *qtedge, GetQtConceptMap());
    }
    else
    {
      assert(!"Do not expect to get here"); //!OCLINT accepted idiom
      item->setSelected(is_selected);
    }
  }
}

void ribi::cmap::CommandDeleteSelected::Undo()
{
  assert(AllHaveSameScene());

  AddDeletedQtNodes();
  AddDeletedQtEdges();

  SetSelected(m_selected_before, true);

  GetQtConceptMap().GetQtToolItem().SetBuddyItem(m_tool_item_old_buddy);
  GetQtConceptMap().GetScene().setFocusItem(m_focus_item_before);

  assert(AllHaveSameScene());

  
}
