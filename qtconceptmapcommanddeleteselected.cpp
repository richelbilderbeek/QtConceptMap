//---------------------------------------------------------------------------
/*
ConceptMap, concept map classes
Copyright (C) 2013-2016 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/CppConceptMap.htm
//---------------------------------------------------------------------------
#include "qtconceptmapcommanddeleteselected.h"

#include <cassert>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "conceptmap.h"
#include "conceptmapedge.h"
#include "conceptmapnode.h"
#include "conceptmapconcept.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmaphelper.h"
#include "remove_selected_custom_edges_and_vertices.h"

ribi::cmap::CommandDeleteSelected::CommandDeleteSelected(
  ConceptMap& conceptmap,
  QGraphicsScene& scene,
  QtTool * const tool_item
)
  : m_conceptmap(conceptmap),
    m_conceptmap_after(conceptmap),
    m_conceptmap_before(conceptmap),
    m_focus_item_before{scene.focusItem()},
    m_qtedges_removed{},
    m_qtnodes_removed{},
    m_scene(scene),
    m_selected_before{scene.selectedItems()},
    m_tool_item{tool_item},
    m_tool_item_old_buddy{tool_item->GetBuddyItem()}
{
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

  for (const auto qtnode: m_qtnodes_removed)
  {
    assert(qtnode->scene());
    assert(!qtnode->parentItem()); //Just to measure
    assert(!qtnode->parentObject()); //Just to measure
    assert(!qtnode->parentWidget()); //Just to measure
  }

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
  for (auto item: m_selected_before) { item->setSelected(false); }
  m_tool_item->SetBuddyItem(nullptr);

  assert(AllHaveSameScene());
}

void ribi::cmap::CommandDeleteSelected::undo()
{
  assert(AllHaveSameScene());

  m_conceptmap = m_conceptmap_before;
  for (const auto qtnode: m_qtnodes_removed)
  {
    assert(qtnode);
    assert(!qtnode->scene());
    assert(!m_scene.items().contains(qtnode));
    m_scene.addItem(qtnode);
    assert(qtnode->scene());
  }
  for (const auto qtedge: m_qtedges_removed)
  {
    assert(qtedge);
    assert(!qtedge->scene());
    assert(!m_scene.items().contains(qtedge));
    m_scene.addItem(qtedge);
    assert(qtedge->scene());

    qtedge->setZValue(-1.0);
  }
  for (auto item: m_selected_before) { item->setSelected(true); }

  m_tool_item->SetBuddyItem(m_tool_item_old_buddy);
  m_scene.setFocusItem(m_focus_item_before);

  assert(AllHaveSameScene());
}
