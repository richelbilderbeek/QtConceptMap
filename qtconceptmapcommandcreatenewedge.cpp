
/*
ConceptMap, concept map classes













*/

//From http://www.richelbilderbeek.nl/CppConceptMap.htm

#include "qtconceptmapcommandcreatenewedge.h"

#include <cassert>
#include <sstream>

#include "conceptmap.h"

#include "conceptmapedgefactory.h"
#include "conceptmapnode.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmaphelper.h"
#include "count_vertices_with_selectedness.h"
#include "qtquadbezierarrowitem.h"
#include "add_edge_between_selected_vertices.h"
#include <boost/graph/isomorphism.hpp>
#include <QGraphicsScene>
#include "qtconceptmap.h"
#include "qtconceptmaphelper.h"
#include "qtconceptmaptoolsitem.h"

ribi::cmap::CommandCreateNewEdgeBetweenTwoSelectedNodes
  ::CommandCreateNewEdgeBetweenTwoSelectedNodes(
  ConceptMap& conceptmap,
  const Mode mode,
  QGraphicsScene& scene,
  QtTool& tool_item
) : m_conceptmap(conceptmap),
    m_added_edge{Edge()},
    m_added_qtedge{nullptr},
    m_added_qtnode{nullptr},
    m_after{conceptmap},
    m_before{conceptmap},
    m_mode{mode},
    m_scene{scene},
    m_selected_before{scene.selectedItems()},
    m_tool_item{tool_item}
{
  if (count_vertices_with_selectedness(true, m_after) != 2)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "Must have two selected vertices, instead of "
      << count_vertices_with_selectedness(true, m_after)
    ;
    throw std::invalid_argument(msg.str());
  }
  this->setText("Create new edge");

  //Create a new edge on the concept map 'm_after'
  const auto ed = add_edge_between_selected_vertices(m_after);
  assert(!boost::isomorphism(m_before,m_after));
  m_added_edge = get(get(boost::edge_custom_type, m_after), ed);

  //Obtain the nodes where this edge was created
  const VertexDescriptor vd_from = boost::source(ed, m_after);
  const VertexDescriptor vd_to = boost::target(ed, m_after);
  assert(vd_from != vd_to);
  const auto vertex_map = get(boost::vertex_custom_type, m_after);
  const Node from = get(vertex_map, vd_from);
  const Node to = get(vertex_map, vd_to);
  assert(from.GetId() != to.GetId());

  //Find the QtNodes where a new QtEdge needs to be created in between
  QtNode * const qtfrom = FindQtNode(from.GetId(), scene);
  QtNode * const qtto = FindQtNode(to.GetId(), scene);

  //Create the new QtEdge
  m_added_qtedge = new QtEdge(m_added_edge, qtfrom, qtto);
  // Cannot write this:
  //   assert(m_added_qtedge->GetEdge() == m_added_edge);
  // as the endpoints of qtedge->GetEdge() have changed
  assert(m_added_qtedge->GetEdge().GetId() == m_added_edge.GetId());
  // Update the original edge
  m_added_edge = m_added_qtedge->GetEdge();
  assert(m_added_qtedge->GetEdge() == m_added_edge);

  //m_qtedge has added a QtNode itself. Store it
  m_added_qtnode = m_added_qtedge->GetQtNode();
}

ribi::cmap::CommandCreateNewEdgeBetweenTwoSelectedNodes
  ::~CommandCreateNewEdgeBetweenTwoSelectedNodes() noexcept
{
  delete m_added_qtedge;
  m_added_qtedge = nullptr;
}

bool ribi::cmap::CommandCreateNewEdgeBetweenTwoSelectedNodes
  ::AllHaveScene(const QGraphicsScene * const scene) noexcept
{
  return m_added_qtedge->scene() == scene
    && m_added_qtnode->scene() == scene
    && m_added_qtedge->GetArrow()->scene() == scene
  ;
}

void ribi::cmap::CommandCreateNewEdgeBetweenTwoSelectedNodes::redo()
{
  m_conceptmap = m_after;
  m_added_qtedge->setFocus();
  m_added_qtedge->SetSelected(true);
  m_added_qtedge->GetFrom()->setSelected(false);
  m_added_qtedge->GetTo()->setSelected(false);

  assert(AllHaveScene(nullptr));
  m_scene.addItem(m_added_qtedge);
  assert(AllHaveScene(&m_scene));

  m_added_qtnode->setFocus();
  m_added_qtnode->setSelected(true);
  m_added_qtnode->SetBrushFunction(GetQtNodeBrushFunction(m_mode));

  // Cannot write this:
  //   assert(m_added_qtedge->GetEdge() == m_added_edge);
  // as the endpoints of qtedge->GetEdge() have changed
  assert(m_added_qtedge->GetEdge().GetId() == m_added_edge.GetId());
  // Update the original edge
  m_added_edge = m_added_qtedge->GetEdge();
  assert(m_added_qtedge->GetEdge() == m_added_edge);

  //Do not create a node on the edge if it is connected to a center node
  if (m_added_qtedge->GetFrom()->GetNode().IsCenterNode()
    || m_added_qtedge->GetTo()->GetNode().IsCenterNode()
  )
  {
    m_added_qtedge->GetQtNode()->setVisible(false);
  }

}

void ribi::cmap::CommandCreateNewEdgeBetweenTwoSelectedNodes::undo()
{
  m_conceptmap = m_before;
  m_added_qtedge->SetSelected(false);
  m_added_qtnode->setSelected(false);
  m_added_qtedge->GetFrom()->setSelected(false);
  m_added_qtedge->GetTo()->setSelected(true);
  m_added_qtedge->GetTo()->setFocus();

  assert(AllHaveScene(&m_scene));
  m_scene.removeItem(m_added_qtedge);
  //m_scene->removeItem(m_added_qtnode); //Get these for free
  //m_scene->removeItem(m_added_qtedge->GetArrow()); //Get these for free
  assert(AllHaveScene(nullptr));

  for (auto item: m_selected_before) { item->setSelected(true); }

  // Cannot write this:
  //   assert(m_added_qtedge->GetEdge() == m_added_edge);
  // as the endpoints of qtedge->GetEdge() have changed
  assert(m_added_qtedge->GetEdge().GetId() == m_added_edge.GetId());
  // Update the original edge
  m_added_edge = m_added_qtedge->GetEdge();
  assert(m_added_qtedge->GetEdge() == m_added_edge);
}
