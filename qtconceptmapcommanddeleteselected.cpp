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
#include "remove_selected_custom_edges_and_vertices.h"

ribi::cmap::CommandDeleteSelected::CommandDeleteSelected(
  QtConceptMap& qtconceptmap
)
  : Command(qtconceptmap),
    m_conceptmap(qtconceptmap.GetConceptMap()),
    m_conceptmap_after(qtconceptmap.GetConceptMap()),
    m_conceptmap_before(qtconceptmap.GetConceptMap()),
    m_focus_item_before{qtconceptmap.GetScene().focusItem()},
    m_qtedges_removed{},
    m_qtnodes_removed{},
    m_scene(qtconceptmap.GetScene()),
    m_selected_before{qtconceptmap.GetScene().selectedItems()},
    m_tool_item{qtconceptmap.GetQtToolItem()},
    m_tool_item_old_buddy{qtconceptmap.GetQtToolItem().GetBuddyItem()}
{
  for (const auto item: m_scene.selectedItems())
  {
    if (IsQtCenterNode(item))
    {
      throw std::invalid_argument("Cannot delete center node");
    }
  }

  setText("delete selected nodes and edges");

  //Count the number of vertices and edges selected
  {
    const int n_vertices_selected{CountSelectedNodes(m_conceptmap_after)};
    const int n_edges_selected{CountSelectedEdges(m_conceptmap_after)};
    if (n_vertices_selected + n_edges_selected == 0) {
      std::stringstream msg;
      msg << __func__ << ": no edges nor vertices selected, cannot delete zero selected items";
      throw std::logic_error(msg.str());
    }
  }

  //Remove the selected vertices from the concept map
  m_conceptmap_after = ::remove_selected_custom_edges_and_vertices(m_conceptmap_after);

  //Find the selected nodes to be deleted
  CollectQtNodesRemoved();

  //Find the edges to be deleted. These can be selected themselves,
  // or connected to a selected qtnode
  CollectQtEdgesRemoved();

  assert(AllHaveSameScene());
}


void ribi::cmap::CommandDeleteSelected::AddToScene(const std::set<QtEdge *>& qtedges)
{
  for (const auto qtedge: qtedges)
  {
    assert(qtedge);
    assert(!qtedge->scene());
    assert(!m_scene.items().contains(qtedge));
    m_scene.addItem(qtedge);
    assert(qtedge->scene());
    qtedge->setZValue(-1.0);
  }
}

void ribi::cmap::CommandDeleteSelected::AddToScene(const std::set<QtNode *>& qtnodes)
{
  for (const auto qtnode: qtnodes)
  {
    assert(qtnode);
    assert(!qtnode->scene());
    assert(!m_scene.items().contains(qtnode));
    m_scene.addItem(qtnode);
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

void ribi::cmap::CommandDeleteSelected::CollectQtEdgesRemoved()
{
  //Find the edges connected to deleted nodes
  for (const auto i: m_scene.items())
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
}

void ribi::cmap::CommandDeleteSelected::CollectQtNodesRemoved()
{
  const auto v = GetSelectedQtNodes(m_scene);
  std::copy(std::begin(v), std::end(v),
    std::inserter(m_qtnodes_removed, std::end(m_qtnodes_removed))
  );

  for (const auto qtnode: m_qtnodes_removed)
  {
    assert(qtnode->scene());
  }
}

void ribi::cmap::CommandDeleteSelected::redo()
{
  assert(AllHaveSameScene());

  m_conceptmap = m_conceptmap_after;

  CheckQtNodesBeforeDelete();

  for (const auto qtedge: m_qtedges_removed)
  {
    assert(qtedge->scene());
    m_scene.removeItem(qtedge);
    assert(!qtedge->scene());
  }
  for (const auto qtnode: m_qtnodes_removed)
  {
    assert(qtnode->scene());
    m_scene.removeItem(qtnode);
    assert(!qtnode->scene());
  }
  SetSelected(m_selected_before, false);

  m_tool_item.SetBuddyItem(nullptr);

  assert(AllHaveSameScene());
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
  assert(AllHaveSameScene());

  m_conceptmap = m_conceptmap_before;
  AddToScene(m_qtnodes_removed);
  AddToScene(m_qtedges_removed);
  SetSelected(m_selected_before, true);

  m_tool_item.SetBuddyItem(m_tool_item_old_buddy);
  m_scene.setFocusItem(m_focus_item_before);

  assert(AllHaveSameScene());
}
