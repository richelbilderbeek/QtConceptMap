#include "qtconceptmapcommandcreatenewedge.h"

#include <cassert>
#include <sstream>
#include <numeric>
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
#include <QDebug>
//#include <QGraphicsItem>
//#include <QGraphicsScene>
#include "qtconceptmap.h"
#include "qtconceptmapqtedge.h"
#include "set_my_custom_edge.h"
#include "qtconceptmaphelper.h"
#include "qtconceptmaptoolsitem.h"

ribi::cmap::CommandCreateNewEdgeBetweenTwoSelectedNodes
  ::CommandCreateNewEdgeBetweenTwoSelectedNodes(
  QtConceptMap& qtconceptmap,
  const std::string& text
) : Command(qtconceptmap),
    m_text{text},
    m_added_qtedge{nullptr}
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
    //&& m_added_qtnode->scene() == scene
    && m_added_qtedge->GetArrow()->scene() == scene
  ;
}

void ribi::cmap::CommandCreateNewEdgeBetweenTwoSelectedNodes::CheckCanRedo() const
{
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

std::pair<int, int> ribi::cmap::GetFromToIds(const EdgeDescriptor ed, const ConceptMap& c)
{
  const VertexDescriptor vd_from = boost::source(ed, c);
  const VertexDescriptor vd_to = boost::target(ed, c);
  assert(vd_from != vd_to);
  const Node from = get_my_custom_vertex(vd_from, c);
  const Node to   = get_my_custom_vertex(vd_to  , c);
  assert(from.GetId() != to.GetId());
  return std::make_pair(from.GetId(), to.GetId());
}


std::pair<ribi::cmap::QtNode*, ribi::cmap::QtNode*>
ribi::cmap::GetFromToQtNodes(
  const EdgeDescriptor,
  const QtConceptMap&)
{
  #ifdef NOT_NOW_20180119
  const std::pair<int, int> from_to_ids = GetFromToIds(ed, q.ToConceptMap());

  //Find the QtNodes where a new QtEdge needs to be created in between
  QtNode * const qtfrom = FindQtNode(from_to_ids.first, q);
  assert(qtfrom);
  QtNode * const qtto = FindQtNode(from_to_ids.second, q);
  assert(qtto);
  assert(qtfrom != qtto);
  return std::make_pair(qtfrom, qtto);
  #endif // NOT_NOW_20180119
  return std::make_pair(nullptr, nullptr);
}

std::string ribi::cmap::GetText(const CommandCreateNewEdgeBetweenTwoSelectedNodes& c) noexcept
{
  return c.GetText();
}

ribi::cmap::CommandCreateNewEdgeBetweenTwoSelectedNodes * ribi::cmap::ParseCommandCreateNewEdge(
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

void ribi::cmap::CommandCreateNewEdgeBetweenTwoSelectedNodes::Redo()
{
  CheckCanRedo(); //Throws if not

  Expects(CountSelectedQtEdges(GetScene(*this)) == 0);
  Expects(CountSelectedQtNodes(GetScene(*this)) == 2);

  GetQtConceptMap().GetScene().clearFocus();

  const auto qtnodes = GetSelectedQtNodes(GetQtConceptMap());
  assert(qtnodes.size() == 2);
  assert(!IsOnEdge(qtnodes[0]));
  assert(!IsOnEdge(qtnodes[1]));

  //Create the new QtEdge
  m_added_qtedge = new QtEdge(
    Concept(m_text),
    (GetX(*qtnodes[0]) + GetX(*qtnodes[1])) / 2.0,
    (GetY(*qtnodes[0]) + GetY(*qtnodes[1])) / 2.0,
    qtnodes[0],
    qtnodes[1]
  );

  //-----------------------
  // Using
  //-----------------------
  assert(AllHaveScene(nullptr));
  GetQtConceptMap().GetScene().addItem(m_added_qtedge);
  assert(AllHaveScene(&GetQtConceptMap().GetScene()));

  m_added_qtedge->setFocus();
  //m_added_qtedge->GetQtNode()->setFocus();
  SetSelectedness(true, *m_added_qtedge, GetQtConceptMap());

  m_added_qtedge->GetQtNode()->SetBrushFunction(
    GetQtNodeBrushFunction(
      GetQtConceptMap().GetMode()
    )
  );

  QtNode * const no_qtnode{nullptr};
  SetQtToolItemBuddy(GetQtConceptMap(), no_qtnode);

  SetSelectedness(false, *m_added_qtedge->GetFrom(), GetQtConceptMap());
  SetSelectedness(false, *m_added_qtedge->GetTo(), GetQtConceptMap());

  // Cannot write this:
  //   assert(m_added_qtedge->GetEdge() == m_added_edge);
  // as the endpoints of qtedge->GetEdge() have changed
  // Update the original edge
  //? added_edge = m_added_qtedge->GetEdge();
  //? assert(m_added_qtedge->GetEdge() == added_edge);

  //Do not create a node on the edge if it is connected to a center node
  if (IsCenterNode(*m_added_qtedge->GetFrom())
    || IsCenterNode(*m_added_qtedge->GetTo())
  )
  {
    m_added_qtedge->GetQtNode()->setVisible(false);
  }
  Ensures(CountSelectedQtEdges(GetScene(*this)) == 1);
  Ensures(CountSelectedQtNodes(GetScene(*this)) == 0);
  Ensures(::ribi::cmap::GetText(*m_added_qtedge) == m_text);
}

void ribi::cmap::CommandCreateNewEdgeBetweenTwoSelectedNodes::Undo()
{
  Expects(CountSelectedQtEdges(GetScene(*this)) == 1);
  Expects(CountSelectedQtNodes(GetScene(*this)) == 0);

  GetQtConceptMap().GetScene().clearFocus();

  SetSelectedness(false, *m_added_qtedge           , GetQtConceptMap());

  //QtConceptMap
  assert(AllHaveScene(&GetQtConceptMap().GetScene()));

  GetQtConceptMap().GetScene().removeItem(m_added_qtedge);
  //GetQtConceptMap().GetScene().removeItem(m_added_qtnode); //Get these for free
  //GetQtConceptMap().GetScene().removeItem(m_added_qtedge->GetArrow()); //Get these for free

  SetSelectedness(true, *m_added_qtedge->GetFrom(), GetQtConceptMap());
  SetSelectedness(true, *m_added_qtedge->GetTo()  , GetQtConceptMap());
  m_added_qtedge->GetTo()->setFocus();

  Ensures(CountSelectedQtEdges(GetScene(*this)) == 0);
  Ensures(CountSelectedQtNodes(GetScene(*this)) == 2);
}
