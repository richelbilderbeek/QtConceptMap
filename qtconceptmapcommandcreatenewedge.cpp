#include "qtconceptmapcommandcreatenewedge.h"

#include <cassert>
#include <sstream>

#include <boost/algorithm/string/trim.hpp>

#include "conceptmap.h"

#include "set_my_bundled_edge.h"
#include "conceptmapedgefactory.h"
#include "conceptmapnode.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmaphelper.h"
#include "count_vertices_with_selectedness.h"
#include "qtquadbezierarrowitem.h"
#include "add_edge_between_selected_vertices.h"
#include <boost/graph/isomorphism.hpp>
#include <QDebug>
#include <QGraphicsScene>
#include "qtconceptmap.h"
#include "qtconceptmaphelper.h"
#include "qtconceptmaptoolsitem.h"

ribi::cmap::CommandCreateNewEdgeBetweenTwoSelectedNodes
  ::CommandCreateNewEdgeBetweenTwoSelectedNodes(
  QtConceptMap& qtconceptmap,
  const std::string& text
) : Command(qtconceptmap),
    m_conceptmap(qtconceptmap.GetConceptMap()),
    m_added_edge{},
    m_added_qtedge{nullptr},
    m_added_qtnode{nullptr},
    m_after{qtconceptmap.GetConceptMap()},
    m_before{qtconceptmap.GetConceptMap()},
    m_mode{qtconceptmap.GetMode()},
    m_scene{qtconceptmap.GetScene()},
    m_selected_before{qtconceptmap.GetScene().selectedItems()},
    m_tool_item{qtconceptmap.GetQtToolItem()}
{
  //assert(GetText(m_added_edge) == text);
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

  //Create a new edge on the concept map 'm_after' with the correct text
  const auto ed = add_edge_between_selected_vertices(m_after);
  auto my_edge_map = get(boost::edge_custom_type, m_after);
  put(my_edge_map, ed, Edge(Node(Concept(text))));

  assert(!boost::isomorphism(m_before,m_after));
  m_added_edge = get(get(boost::edge_custom_type, m_after), ed);

  assert(GetText(m_added_edge) == text);

  //Obtain the nodes where this edge was created
  const VertexDescriptor vd_from = boost::source(ed, m_after);
  const VertexDescriptor vd_to = boost::target(ed, m_after);
  assert(vd_from != vd_to);
  const auto vertex_map = get(boost::vertex_custom_type, m_after);
  const Node from = get(vertex_map, vd_from);
  const Node to = get(vertex_map, vd_to);
  assert(from.GetId() != to.GetId());

  //Find the QtNodes where a new QtEdge needs to be created in between
  QtNode * const qtfrom = FindQtNode(from.GetId(), m_scene);
  QtNode * const qtto = FindQtNode(to.GetId(), m_scene);

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

  //Post-condition
  assert(GetText(*this) == text);
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

std::string ribi::cmap::GetText(const CommandCreateNewEdgeBetweenTwoSelectedNodes& c) noexcept
{
  return GetText(c.GetQtEdge());
}

ribi::cmap::CommandCreateNewEdgeBetweenTwoSelectedNodes * ribi::cmap::parse_command_create_new_edge(
  QtConceptMap& qtconceptmap, std::string s)
{
  //"create_new_edge(my text)"
  boost::algorithm::trim(s);
  const std::string str_begin = "create_new_edge(";
  if (s.substr(0, str_begin.size()) != str_begin) return nullptr;
  if (s.back() != ')') return nullptr;
  //"text"
  const std::string t = s.substr(str_begin.size(), s.size() - str_begin.size() - 1);
  assert(t[0] != '(');
  assert(t.back() != ')');
  // "my text"
  return new CommandCreateNewEdgeBetweenTwoSelectedNodes(qtconceptmap, t);
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
