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
#include "qtconceptmaptoolsitem.h"
#include "qtquadbezierarrowitem.h"

ribi::cmap::CommandDeleteSelected::CommandDeleteSelected(
  QtConceptMap& qtconceptmap
)
  : Command(qtconceptmap),
    m_focus_item_before{qtconceptmap.GetScene().focusItem()},
    m_selected_qtedges_removed{},
    m_qtnodes_removed{},
    m_tool_item_old_buddy{qtconceptmap.GetQtToolItem().GetBuddyItem()}
{
  setText("delete selected nodes and edges");
}

ribi::cmap::CommandDeleteSelected::~CommandDeleteSelected()
{

}

void ribi::cmap::CommandDeleteSelected::AddDeletedQtEdges()
{
  for (const auto qtedge: m_selected_qtedges_removed)
  {
    assert(qtedge);
    assert(!qtedge->scene());
    assert(!qtedge->GetArrow()->scene());
    GetQtConceptMap().GetScene().addItem(qtedge);
    SetSelectedness(true, *qtedge);
    assert(qtedge->scene());
    assert(qtedge->GetArrow()->scene());
    assert(qtedge->zValue() == GetQtEdgeZvalue());
  }
  for (const auto qtedge: m_unselected_qtedges_removed)
  {
    assert(qtedge);
    assert(!qtedge->scene());
    assert(!qtedge->GetArrow()->scene());
    GetQtConceptMap().GetScene().addItem(qtedge);
    assert(qtedge->scene());
    assert(qtedge->GetArrow()->scene());
    assert(qtedge->zValue() == GetQtEdgeZvalue());
  }
}

void ribi::cmap::CommandDeleteSelected::AddDeletedQtNodes()
{
  for (const auto qtnode: m_qtnodes_removed)
  {
    assert(qtnode);
    assert(!qtnode->scene());
    assert(!GetQtConceptMap().GetScene().items().contains(qtnode));
    qtnode->setSelected(true);
    GetQtConceptMap().GetScene().addItem(qtnode);
    assert(qtnode->scene());
    assert(qtnode->zValue() == GetQtNodeZvalue());
  }
}

bool ribi::cmap::CommandDeleteSelected::AllHaveSameScene() const noexcept
{
  std::set<QGraphicsScene*> s;
  std::transform(
    std::begin(m_selected_qtedges_removed),
    std::end(m_selected_qtedges_removed),
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


void ribi::cmap::CommandDeleteSelected::RemoveQtEdges()
{
  m_selected_qtedges_removed = GetSelectedQtEdges(GetQtConceptMap());
  m_unselected_qtedges_removed = {};


  for (QtEdge * qtedge: GetQtEdges(GetQtConceptMap()))
  {
    if (IsSelected(*qtedge)) continue;
    if (qtedge->GetFrom()->isSelected() || qtedge->GetTo()->isSelected())
    {
      m_unselected_qtedges_removed.push_back(qtedge);
    }
  }

  std::set<QtEdge *> qtedges_to_remove(
    std::begin(m_selected_qtedges_removed),
    std::end(m_selected_qtedges_removed)
  );
  std::copy(
    std::begin(m_unselected_qtedges_removed),
    std::end(m_unselected_qtedges_removed),
    std::inserter(qtedges_to_remove, std::begin(qtedges_to_remove))
  );
  assert(qtedges_to_remove.size()
    == m_unselected_qtedges_removed.size() + m_selected_qtedges_removed.size()
  );

  for (QtEdge * const qtedge: qtedges_to_remove)
  {
    assert(qtedge);
    SetSelectedness(false, *qtedge);
    assert(qtedge->scene());
    assert(qtedge->GetArrow()->scene());
    GetQtConceptMap().GetScene().removeItem(qtedge);
    assert(!qtedge->scene());
    assert(!qtedge->GetArrow()->scene());
  }
}

void ribi::cmap::CommandDeleteSelected::RemoveSelectedQtNodes()
{
  m_qtnodes_removed.clear();
  for (QtNode * const qtnode: GetSelectedQtNodes(GetQtConceptMap()))
  {
    assert(!IsOnEdge(*qtnode));
    if (!IsQtCenterNode(*qtnode))
    {
      m_qtnodes_removed.push_back(qtnode);
    }
  }
  for (QtNode * const qtnode: m_qtnodes_removed)
  {
    assert(!IsOnEdge(*qtnode));
    assert(!IsQtCenterNode(qtnode));
    SetSelectedness(false, *qtnode);
    GetScene(*this).removeItem(qtnode);
    assert(!qtnode->scene());
  }
}

void ribi::cmap::CommandDeleteSelected::Redo()
{
  #ifdef GET_SELECTION_IN_REDO_20180823
  m_selected_before = qtconceptmap.GetScene().selectedItems();
  #endif // GET_SELECTION_IN_REDO_20180823

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
  RemoveQtEdges();
  RemoveSelectedQtNodes();
  assert(AllHaveSameScene());
}

void ribi::cmap::CommandDeleteSelected::Undo()
{
  assert(AllHaveSameScene());

  AddDeletedQtNodes();
  AddDeletedQtEdges();

  GetQtConceptMap().GetQtToolItem().SetBuddyItem(m_tool_item_old_buddy);
  GetQtConceptMap().GetScene().setFocusItem(m_focus_item_before);

  assert(AllHaveSameScene());


}
