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

ribi::cmap::CommandDeleteSelected::CommandDeleteSelected(
  QtConceptMap& qtconceptmap
)
  : Command(qtconceptmap),
    m_focus_item_before{qtconceptmap.GetScene().focusItem()},
    m_selected_qtedges_removed{},
    m_qtnodes_removed{},
    m_selected_before{qtconceptmap.GetScene().selectedItems()},
    m_tool_item_old_buddy{qtconceptmap.GetQtToolItem().GetBuddyItem()}
{
  setText("delete selected nodes and edges");
}

ribi::cmap::CommandDeleteSelected::~CommandDeleteSelected()
{

}

void ribi::cmap::CommandDeleteSelected::AddDeletedQtEdges()
{
  std::vector<QtEdge *> qtedges_removed = m_selected_qtedges_removed;
  std::copy(std::begin(m_unselected_qtedges_removed), std::end(m_unselected_qtedges_removed),
    std::back_inserter(qtedges_removed)
  );

  for (const auto qtedge: qtedges_removed)
  {
    assert(qtedge);
    GetQtConceptMap().GetScene().addItem(qtedge);
    assert(qtedge->scene());
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

void ribi::cmap::CommandDeleteSelected::RemoveQtEdges()
{
  m_selected_qtedges_removed = GetSelectedQtEdges(GetQtConceptMap());
  m_unselected_qtedges_removed = {};

  for (QtEdge * qtedge: GetQtEdges(GetQtConceptMap()))
  {
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

  for (QtEdge * const qtedge: qtedges_to_remove)
  {
    SetSelectedness(false, *qtedge);
    GetQtConceptMap().GetScene().removeItem(qtedge);
    assert(!qtedge->scene());
  }
}

void ribi::cmap::CommandDeleteSelected::RemoveSelectedQtNodes()
{
  m_qtnodes_removed = GetSelectedQtNodes(GetQtConceptMap());
  for (QtNode * const qtnode: GetSelectedQtNodes(GetQtConceptMap()))
  {
    SetSelectedness(false, *qtnode);
    GetScene(*this).removeItem(qtnode);
    assert(!qtnode->scene());
  }
}

void ribi::cmap::CommandDeleteSelected::Redo()
{
  qDebug() << "REDO";
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

/*
void ribi::cmap::CommandDeleteSelected::SelectAllQtEdgesOnSelectedQtNodes()
{
  for (QtEdge * qtedge: GetQtEdges(GetQtConceptMap()))
  {
    if (qtedge->GetFrom()->isSelected())
    {
      SetSelectedness(true, *qtedge);
    }
    else if (qtedge->GetTo()->isSelected())
    {
      SetSelectedness(true, *qtedge);
    }
  }
}
*/

void ribi::cmap::CommandDeleteSelected::SetSelected(
  const QList<QGraphicsItem *>& v,
  const bool is_selected
) noexcept
{
  for (auto item: v)
  {
    if (QtNode* const qtnode = qgraphicsitem_cast<QtNode*>(item))
    {
      SetSelectedness(is_selected, *qtnode);
    }
    else if (QtEdge* const qtedge = qgraphicsitem_cast<QtEdge*>(item))
    {
      SetSelectedness(is_selected, *qtedge);
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
