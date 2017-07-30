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
#include "find_first_custom_edge_with_my_edge.h"
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
    m_qtconceptmap(qtconceptmap),
    m_text{text},
    m_added_qtedge{nullptr},
    m_added_qtnode{nullptr},
    m_qtfrom{nullptr},
    m_qtto{nullptr},
    m_selected_before{}
{
  this->setText("Create new edge between two selected nodes");
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
  return c.GetText();
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
  if (CountSelectedQtNodes(m_qtconceptmap)
    != count_vertices_with_selectedness(true, m_qtconceptmap.GetConceptMap()))
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "Must have as much selected vertices ("
      << count_vertices_with_selectedness(true, m_qtconceptmap.GetConceptMap())
      << ") as selected QtNodes (" << CountSelectedQtNodes(m_qtconceptmap) << ")"
    ;
    throw std::invalid_argument(msg.str());
  }

  assert(CountSelectedQtNodes(m_qtconceptmap)
    == count_vertices_with_selectedness(true, m_qtconceptmap.GetConceptMap()));

  if (CountSelectedQtNodes(m_qtconceptmap) != 2)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "Must have two selected vertices, instead of "
      << CountSelectedQtNodes(m_qtconceptmap)
    ;
    throw std::invalid_argument(msg.str());
  }

  m_selected_before = m_qtconceptmap.GetScene().selectedItems();
  //-------------
  // Concept map
  //-------------
  const ConceptMap concept_map_before = m_qtconceptmap.GetConceptMap();
  ConceptMap& concept_map = m_qtconceptmap.GetConceptMap();

  //Create a new edge on the concept map 'm_after' with the correct text
  const auto ed = add_edge_between_selected_vertices(concept_map);
  auto my_edge_map = get(boost::edge_custom_type, concept_map);
  put(my_edge_map, ed, Edge(Node(Concept(m_text))));

  assert(!boost::isomorphism(concept_map_before, concept_map));
  Edge added_edge = get(get(boost::edge_custom_type, concept_map), ed);
  assert(::ribi::cmap::GetText(added_edge) == m_text);

  //Obtain the nodes where this edge was created
  const VertexDescriptor vd_from = boost::source(ed, concept_map);
  const VertexDescriptor vd_to = boost::target(ed, concept_map);
  assert(vd_from != vd_to);
  const auto vertex_map = get(boost::vertex_custom_type, concept_map);
  const Node from = get(vertex_map, vd_from);
  const Node to = get(vertex_map, vd_to);
  assert(from.GetId() != to.GetId());

  //-------------
  // QtConceptMap
  //-------------
  //Find the QtNodes where a new QtEdge needs to be created in between
  QtNode * const qtfrom = FindQtNode(from.GetId(), m_qtconceptmap.GetScene());
  assert(qtfrom);
  QtNode * const qtto = FindQtNode(to.GetId(), m_qtconceptmap.GetScene());
  assert(qtto);

  //Create the new QtEdge
  m_added_qtedge = new QtEdge(added_edge, qtfrom, qtto);
  // Cannot write this:
  //   assert(m_added_qtedge->GetEdge() == m_added_edge);
  // as the endpoints of qtedge->GetEdge() have changed
  assert(m_added_qtedge->GetEdge().GetId() == added_edge.GetId());
  // Update the original edge
  added_edge = m_added_qtedge->GetEdge();
  assert(m_added_qtedge->GetEdge() == added_edge);

  //m_qtedge has added a QtNode itself. Store it
  m_added_qtnode = m_added_qtedge->GetQtNode();


  //-----------------------
  // Using
  //-----------------------
  m_added_qtedge->setFocus();
  m_added_qtedge->SetSelected(true);
  m_added_qtedge->GetFrom()->setSelected(false);
  m_added_qtedge->GetTo()->setSelected(false);

  assert(AllHaveScene(nullptr));
  m_qtconceptmap.GetScene().addItem(m_added_qtedge);
  assert(AllHaveScene(&m_qtconceptmap.GetScene()));

  m_added_qtnode->setFocus();
  m_added_qtnode->setSelected(true);
  m_added_qtnode->SetBrushFunction(
    GetQtNodeBrushFunction(
      m_qtconceptmap.GetMode()
    )
  );

  // Cannot write this:
  //   assert(m_added_qtedge->GetEdge() == m_added_edge);
  // as the endpoints of qtedge->GetEdge() have changed
  assert(m_added_qtedge->GetEdge().GetId() == added_edge.GetId());
  // Update the original edge
  added_edge = m_added_qtedge->GetEdge();
  assert(m_added_qtedge->GetEdge() == added_edge);

  //Do not create a node on the edge if it is connected to a center node
  if (m_added_qtedge->GetFrom()->GetNode().IsCenterNode()
    || m_added_qtedge->GetTo()->GetNode().IsCenterNode()
  )
  {
    m_added_qtedge->GetQtNode()->setVisible(false);
  }

  //Post-condition: the text must be on the added QtEdge
  assert(::ribi::cmap::GetText(*m_added_qtedge) == m_text);
}

void ribi::cmap::CommandCreateNewEdgeBetweenTwoSelectedNodes::undo()
{
  //ConceptMap
  boost::remove_edge(
    find_first_custom_edge_with_my_edge(m_added_qtedge->GetEdge(), m_qtconceptmap.GetConceptMap()),
    m_qtconceptmap.GetConceptMap()
  );

  //QtConceptMap
  assert(AllHaveScene(&m_qtconceptmap.GetScene()));
  m_qtconceptmap.GetScene().removeItem(m_added_qtedge);
  //m_qtconceptmap.GetScene().removeItem(m_added_qtnode); //Get these for free
  //m_qtconceptmap.GetScene().removeItem(m_added_qtedge->GetArrow()); //Get these for free
  assert(AllHaveScene(nullptr));
  m_added_qtedge->SetSelected(false);
  m_added_qtnode->setSelected(false);
  m_added_qtedge->GetFrom()->setSelected(false);
  m_added_qtedge->GetTo()->setSelected(true);
  m_added_qtedge->GetTo()->setFocus();

  for (auto item: m_selected_before) { item->setSelected(true); }
}
