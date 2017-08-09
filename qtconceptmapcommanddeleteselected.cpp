#include "qtconceptmapcommanddeleteselected.h"

#include <cassert>
#include <iostream>
#include <sstream>
#include <stdexcept>

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

ribi::cmap::CommandDeleteSelected::CommandDeleteSelected(
  QtConceptMap& qtconceptmap
)
  : Command(qtconceptmap),
    m_conceptmap(qtconceptmap.GetConceptMap()),
    //m_conceptmap_after(qtconceptmap.GetConceptMap()),
    //m_conceptmap_before(qtconceptmap.GetConceptMap()),
    m_focus_item_before{qtconceptmap.GetScene().focusItem()},
    m_qtedges_removed{},
    m_qtnodes_removed{},
    //m_scene(qtconceptmap.GetScene()),
    m_selected_before{qtconceptmap.GetScene().selectedItems()},
    //m_tool_item{qtconceptmap.GetQtToolItem()},
    m_tool_item_old_buddy{qtconceptmap.GetQtToolItem().GetBuddyItem()}
{
  setText("delete selected nodes and edges");
}


void ribi::cmap::CommandDeleteSelected::AddDeletedQtEdges()
{
  for (const auto qtedge: m_qtedges_removed)
  {
    assert(qtedge);
    assert(!qtedge->scene());
    assert(!GetScene(*this).items().contains(qtedge));
    GetScene(*this).addItem(qtedge);
    assert(qtedge->scene());
    qtedge->setZValue(-1.0);
  }
}

void ribi::cmap::CommandDeleteSelected::AddDeletedQtNodes()
{
  for (const auto qtnode: m_qtnodes_removed)
  {
    assert(qtnode);
    assert(!qtnode->scene());
    assert(!GetScene(*this).items().contains(qtnode));
    GetScene(*this).addItem(qtnode);
    assert(qtnode->scene());
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
    const auto ed = find_first_custom_edge_with_my_edge(qtedge->GetEdge(), GetConceptMap(*this));
    boost::remove_edge(ed, GetConceptMap(*this));
    GetQtConceptMap().GetScene().removeItem(qtedge);
  }

  #ifdef KEEP_THIS_LOVELY_OBSOLETE_CODE_20170809
  //Find the edges connected to deleted nodes
  for (const auto i: GetQtConceptMap().GetScene().items())
  {
    assert(i->scene());
    if (QtEdge* qtedge = dynamic_cast<QtEdge*>(i))
    {
      //Is selected itself
      if (qtedge->isSelected())
      {
        assert(qtedge->scene());
        m_qtedges_removed.insert(qtedge);
        continue;
      }
      //Is connected to a deleted QtNode
      const auto j = std::find_if(
        std::begin(m_qtnodes_removed),std::end(m_qtnodes_removed),
        [qtedge](const QtNode* const qtnode) {
          return qtedge->GetFrom() == qtnode
            || qtedge->GetTo() == qtnode
          ;
        }
      );
      if (j != std::end(m_qtnodes_removed))
      {
        assert(qtedge->scene());
        m_qtedges_removed.insert(qtedge);
      }
    }
  }

  //Items must be in scene before deletion
  for (const auto qtedge: m_qtedges_removed)
  {
    assert(qtedge->scene());
  }
  #endif
}

void ribi::cmap::CommandDeleteSelected::RemoveSelectedQtNodes()
{
  m_qtnodes_removed = GetSelectedQtNodes(GetQtConceptMap());
  for (QtNode * const qtnode: GetSelectedQtNodes(GetQtConceptMap()))
  {
    SetSelectedness(false, *qtnode, GetQtConceptMap());
    const auto vd = find_first_custom_vertex_with_my_vertex(qtnode->GetNode(), GetConceptMap(*this));
    boost::remove_vertex(vd, GetConceptMap(*this));
    GetQtConceptMap().GetScene().removeItem(qtnode);
  }


  #ifdef KEEP_THIS_LOVELY_OBSOLETE_CODE_20170809
  const auto v = GetSelectedQtNodes(GetQtConceptMap().GetScene());
  std::copy(std::begin(v), std::end(v),
    std::inserter(m_qtnodes_removed, std::end(m_qtnodes_removed))
  );

  for (const auto qtnode: m_qtnodes_removed)
  {
    assert(qtnode->scene());
  }
  #endif // KEEP_THIS_LOVELY_OBSOLETE_CODE_20170809
}

void ribi::cmap::CommandDeleteSelected::redo()
{
  CheckInvariants(GetQtConceptMap());
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

  //Remove the selected vertices from the concept map

  //Find the selected nodes to be deleted
  //CollectQtNodesRemoved();

  //Find the edges to be deleted. These can be selected themselves,
  // or connected to a selected qtnode
  //CollectQtEdgesRemoved();

  assert(AllHaveSameScene());

  GetQtConceptMap().GetQtToolItem().SetBuddyItem(nullptr);

  RemoveSelectedQtEdges();
  RemoveSelectedQtNodes();

  assert(AllHaveSameScene());

  CheckInvariants(GetQtConceptMap());
}

void ribi::cmap::CommandDeleteSelected::SetSelected(
  const QList<QGraphicsItem *>& v,
  const bool is_selected
) const noexcept
{
  for (auto item: v)
  {
    item->setSelected(is_selected);
  }
}

void ribi::cmap::CommandDeleteSelected::undo()
{
  CheckInvariants(GetQtConceptMap());

  assert(AllHaveSameScene());

  AddDeletedQtNodes();
  AddDeletedQtNodes();
  SetSelected(m_selected_before, true);

  GetQtConceptMap().GetQtToolItem().SetBuddyItem(m_tool_item_old_buddy);
  GetQtConceptMap().GetScene().setFocusItem(m_focus_item_before);

  assert(AllHaveSameScene());

  CheckInvariants(GetQtConceptMap());
}
