#include "qtconceptmapcommandcreatenewedge.h"

#include <cassert>
#include <sstream>

#include <gsl/gsl_assert>
#include <boost/algorithm/string/trim.hpp>

#include "conceptmap.h"

#include "set_my_bundled_edge.h"
#include "conceptmapedgefactory.h"
#include "conceptmapnode.h"
#include "qtconceptmapqtnode.h"
#include "get_my_custom_edge.h"
#include "get_my_custom_vertex.h"
#include "qtconceptmaphelper.h"
#include "count_vertices_with_selectedness.h"
#include "qtquadbezierarrowitem.h"
#include "add_edge_between_selected_vertices.h"
#include "find_first_custom_edge_with_my_edge.h"
#include <boost/graph/isomorphism.hpp>
#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include "qtconceptmap.h"
#include "set_my_custom_edge.h"
#include "qtconceptmaphelper.h"
#include "qtconceptmaptoolsitem.h"

ribi::cmap::CommandCreateNewEdgeBetweenTwoSelectedNodes
  ::CommandCreateNewEdgeBetweenTwoSelectedNodes(
  QtConceptMap& qtconceptmap,
  const std::string& text
) : Command(qtconceptmap),
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
  // delete m_added_qtedge;//NO! No ownership
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

void ribi::cmap::CommandCreateNewEdgeBetweenTwoSelectedNodes::CheckCanRedo() const
{
  if (CountSelectedQtNodes(GetQtConceptMap())
    != count_vertices_with_selectedness(true, GetQtConceptMap().GetConceptMap()))
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "Must have as much selected vertices ("
      << count_vertices_with_selectedness(true, GetQtConceptMap().GetConceptMap())
      << ") as selected QtNodes (" << CountSelectedQtNodes(GetQtConceptMap()) << ")"
    ;
    throw std::invalid_argument(msg.str());
  }

  assert(CountSelectedQtNodes(GetQtConceptMap())
    == count_vertices_with_selectedness(true, GetQtConceptMap().GetConceptMap()));

  if (CountSelectedQtNodes(GetQtConceptMap()) != 2)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "Must have two selected vertices, instead of "
      << CountSelectedQtNodes(GetQtConceptMap())
    ;
    throw std::invalid_argument(msg.str());
  }
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
  CheckCanRedo(); //Throws if not
  CheckInvariants(GetQtConceptMap());

  m_selected_before = GetQtConceptMap().GetScene().selectedItems();

  //-------------
  // Concept map
  //-------------
  const ConceptMap concept_map_before = GetQtConceptMap().GetConceptMap();
  ConceptMap& concept_map = GetQtConceptMap().GetConceptMap();


  double midx{0.0};
  double midy{0.0};
  {
    const auto vds = get_vertices_with_selectedness(true, concept_map);
    assert(vds.size() == 2);
    const int x1 = GetX(get_my_custom_vertex(vds[0], concept_map));
    const int y1 = GetY(get_my_custom_vertex(vds[0], concept_map));
    const int x2 = GetX(get_my_custom_vertex(vds[1], concept_map));
    const int y2 = GetY(get_my_custom_vertex(vds[1], concept_map));
    midx = (x1 + x2) / 2.0;
    midy = (y1 + y2) / 2.0;
  }

  //Create a new edge on the concept map 'm_after' with the correct text
  const auto ed = add_edge_between_selected_vertices(concept_map);
  set_my_custom_edge(
    Edge(Node(Concept(m_text), false, midx, midy)), ed, GetQtConceptMap().GetConceptMap());

  assert(!boost::isomorphism(concept_map_before, concept_map));
  assert(::ribi::cmap::GetText(
    get_my_custom_edge(ed, GetQtConceptMap().GetConceptMap())) == m_text);

  //Obtain the nodes where this edge was created
  const VertexDescriptor vd_from = boost::source(ed, concept_map);
  const VertexDescriptor vd_to = boost::target(ed, concept_map);
  assert(vd_from != vd_to);
  const Node from = get_my_custom_vertex(vd_from, GetQtConceptMap().GetConceptMap());
  const Node to   = get_my_custom_vertex(vd_to  , GetQtConceptMap().GetConceptMap());
  assert(from.GetId() != to.GetId());

  //-------------
  // QtConceptMap
  //-------------
  //Find the QtNodes where a new QtEdge needs to be created in between
  QtNode * const qtfrom = FindQtNode(from.GetId(), GetQtConceptMap().GetScene());
  assert(qtfrom);
  QtNode * const qtto = FindQtNode(to.GetId(), GetQtConceptMap().GetScene());
  assert(qtto);

  //Create the new QtEdge
  m_added_qtedge = new QtEdge(
    get_my_custom_edge(ed, GetQtConceptMap().GetConceptMap()),
    qtfrom, qtto);
  // Cannot write this:
  //   assert(m_added_qtedge->GetEdge() == m_added_edge);
  // as the endpoints of qtedge->GetEdge() have changed
  assert(m_added_qtedge->GetEdge().GetId()
    == get_my_custom_edge(ed, GetQtConceptMap().GetConceptMap()).GetId());
  // Update the original edge
  //? m_added_edge = m_added_qtedge->GetEdge();
  //? assert(m_added_qtedge->GetEdge() == added_edge);

  //m_qtedge has added a QtNode itself. Store it
  m_added_qtnode = m_added_qtedge->GetQtNode();

  //-----------------------
  // Using
  //-----------------------
  CheckInvariantAsMuchNodesAsQtNodesSelected(GetQtConceptMap());

  m_added_qtedge->setFocus();

  SetSelectedness(true, *m_added_qtedge, GetQtConceptMap());

  assert(AllHaveScene(nullptr));
  GetQtConceptMap().GetScene().addItem(m_added_qtedge);
  assert(AllHaveScene(&GetQtConceptMap().GetScene()));

  m_added_qtnode->setFocus();

  CheckInvariantAsMuchNodesAsQtNodesSelected(GetQtConceptMap());

  SetSelectedness(true, *m_added_qtnode, GetQtConceptMap());

  m_added_qtnode->SetBrushFunction(
    GetQtNodeBrushFunction(
      GetQtConceptMap().GetMode()
    )
  );
  SetQtToolItemBuddy(GetQtConceptMap(), m_added_qtnode);
  CheckInvariantSingleSelectedQtNodeMustHaveQtTool(GetQtConceptMap());

  CheckInvariantAsMuchNodesAsQtNodesSelected(GetQtConceptMap());

  SetSelectedness(false, *m_added_qtedge->GetFrom(), GetQtConceptMap());

  CheckInvariantAsMuchNodesAsQtNodesSelected(GetQtConceptMap());

  SetSelectedness(false, *m_added_qtedge->GetTo()  , GetQtConceptMap());

  CheckInvariantAsMuchNodesAsQtNodesSelected(GetQtConceptMap());

  CheckInvariantQtEdgesAndEdgesHaveSameCoordinats(GetQtConceptMap());

  // Cannot write this:
  //   assert(m_added_qtedge->GetEdge() == m_added_edge);
  // as the endpoints of qtedge->GetEdge() have changed
  // Update the original edge
  //? added_edge = m_added_qtedge->GetEdge();
  //? assert(m_added_qtedge->GetEdge() == added_edge);

  //Do not create a node on the edge if it is connected to a center node
  if (m_added_qtedge->GetFrom()->GetNode().IsCenterNode()
    || m_added_qtedge->GetTo()->GetNode().IsCenterNode()
  )
  {
    m_added_qtedge->GetQtNode()->setVisible(false);
  }


  //Post-condition: the text must be on the added QtEdge
  CheckInvariantAsMuchNodesAsQtNodesSelected(GetQtConceptMap());
  Ensures(::ribi::cmap::GetText(*m_added_qtedge) == m_text);
  Ensures(CountSelectedQtNodes(GetQtConceptMap())
    == count_vertices_with_selectedness(true, GetQtConceptMap().GetConceptMap()));
  CheckInvariants(GetQtConceptMap());
}

void ribi::cmap::CommandCreateNewEdgeBetweenTwoSelectedNodes::undo()
{
  CheckInvariants(GetQtConceptMap());

  //ConceptMap
  boost::remove_edge(
    find_first_custom_edge_with_my_edge(m_added_qtedge->GetEdge(), GetQtConceptMap().GetConceptMap()),
    GetQtConceptMap().GetConceptMap()
  );

  //QtConceptMap
  assert(AllHaveScene(&GetQtConceptMap().GetScene()));
  GetQtConceptMap().GetScene().removeItem(m_added_qtedge);
  //GetQtConceptMap().GetScene().removeItem(m_added_qtnode); //Get these for free
  //GetQtConceptMap().GetScene().removeItem(m_added_qtedge->GetArrow()); //Get these for free
  assert(AllHaveScene(nullptr));
  m_added_qtedge->SetSelected(false);
  m_added_qtnode->setSelected(false);
  m_added_qtedge->GetFrom()->setSelected(false);
  m_added_qtedge->GetTo()->setSelected(true);
  m_added_qtedge->GetTo()->setFocus();

  for (auto item: m_selected_before) { item->setSelected(true); }

  //Post-conditions
  Ensures(CountSelectedQtNodes(GetQtConceptMap())
    == count_vertices_with_selectedness(true, GetQtConceptMap().GetConceptMap()));

  CheckInvariants(GetQtConceptMap());
}
