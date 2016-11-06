#include "qtconceptmapcommandcreatenewnode.h"

#include <cassert>
#include <boost/graph/isomorphism.hpp>

#include "conceptmap.h"
#include "conceptmapnode.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmaphelper.h"

ribi::cmap::CommandCreateNewNode::CommandCreateNewNode(
  ConceptMap& conceptmap,
  const Mode mode,
  QGraphicsScene& scene,
  QtTool& tool_item,
  const double x,
  const double y
)
  : m_conceptmap(conceptmap),
    m_conceptmap_after{conceptmap},
    m_conceptmap_before{conceptmap},
    m_mode{mode},
    m_qtnode{nullptr},
    m_scene(scene),
    m_tool_item{tool_item},
    m_tool_item_old_buddy{tool_item.GetBuddyItem()},
    m_x{x},
    m_y{y}
{


  this->setText("Create new node");

  //Add the vertex to the concept map
  VertexDescriptor vd = boost::add_vertex(m_conceptmap_after);

  //Create the node
  Node node;
  node.SetX(m_x);
  node.SetY(m_y);

  //Set the node
  {
    const auto pmap = get(boost::vertex_custom_type, m_conceptmap_after);
    put(pmap, vd, node);
  }
  //Additively select node
  {
    const auto pmap = get(boost::vertex_is_selected, m_conceptmap_after);
    put(pmap, vd, true);
  }

  //Modify the QGraphicsScene
  m_qtnode = new QtNode(node);
  assert(m_qtnode);
  assert(m_qtnode->GetCenterX() == node.GetX());
  assert(m_qtnode->GetCenterY() == node.GetY());
}

void ribi::cmap::CommandCreateNewNode::redo()
{
  m_conceptmap = m_conceptmap_after;
  assert(!m_qtnode->scene());
  m_scene.addItem(m_qtnode);
  assert(m_qtnode->scene());
  m_qtnode->setSelected(true); //Additively select node
  m_qtnode->setFocus();
  m_qtnode->SetBrushFunction(GetQtNodeBrushFunction(m_mode));
  m_tool_item.SetBuddyItem(m_qtnode);
}

void ribi::cmap::CommandCreateNewNode::undo()
{
  m_conceptmap = m_conceptmap_before;
  assert(m_qtnode->scene());
  m_scene.removeItem(m_qtnode);
  assert(!m_qtnode->scene());
  m_tool_item.SetBuddyItem(m_tool_item_old_buddy);
}
