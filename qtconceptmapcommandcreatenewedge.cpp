#include "qtconceptmapcommandcreatenewedge.h"

#include <cassert>
#include <sstream>
#include <numeric>
#include <gsl/assert>
#include <boost/algorithm/string/trim.hpp>

#include "conceptmap.h"

#include "set_my_bundled_edge.h"
#include "conceptmapedgefactory.h"
#include "conceptmapnode.h"
#include "qtconceptmapqtnode.h"
#include "get_my_bundled_edge.h"
#include "get_my_bundled_vertex.h"
#include "qtconceptmaphelper.h"
#include "count_vertices_with_selectedness.h"
#include "qtquadbezierarrowitem.h"
#include "add_edge_between_selected_vertices.h"
#include "find_first_bundled_edge_with_my_edge.h"
#include "qtconceptmap.h"
#include "qtconceptmapqtedge.h"
#include "set_my_bundled_edge.h"
#include "qtconceptmaphelper.h"
#include "qtconceptmaptoolsitem.h"

#include <QBrush>

ribi::cmap::CommandCreateNewEdge
  ::CommandCreateNewEdge(
  QtConceptMap& qtconceptmap,
  const std::string& text
) : CommandCreateNewEdge(
  qtconceptmap,
  GetSelectedQtNodes(qtconceptmap.GetScene()).at(0),
  GetSelectedQtNodes(qtconceptmap.GetScene()).at(1),
  text
)
{
  //Done
}

ribi::cmap::CommandCreateNewEdge
  ::CommandCreateNewEdge(
  QtConceptMap& qtconceptmap,
  QtNode * const from,
  QtNode * const to,
  const std::string& text
) : Command(qtconceptmap),
    m_added_qtedge{nullptr},
    m_from{from},
    m_text{text},
    m_to{to}
{
  assert(m_from);
  assert(m_to);
  assert(!IsOnEdge(*m_from));
  assert(!IsOnEdge(*m_to));

  this->setText(
    QString("Create new edge. From '")
    + QString::fromStdString(::ribi::cmap::GetText(*from))
    + QString("' to '")
    + QString::fromStdString(::ribi::cmap::GetText(*to))
    + "' with text '"
    + QString::fromStdString(text) + "'"
  );
}

ribi::cmap::CommandCreateNewEdge
  ::~CommandCreateNewEdge() noexcept
{
  // delete m_added_qtedge;//NO! No ownership
  m_added_qtedge = nullptr;
}

bool ribi::cmap::CommandCreateNewEdge
  ::AllHaveScene(const QGraphicsScene * const scene) noexcept
{
  return m_added_qtedge->scene() == scene
    //&& m_added_qtnode->scene() == scene
    && m_added_qtedge->GetArrow()->scene() == scene
  ;
}

void ribi::cmap::CommandCreateNewEdge::CheckCanRedo() const
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
  if (CountSelectedQtEdges(GetQtConceptMap()) != 0)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "Must have zero selected edges, instead of "
      << CountSelectedQtEdges(GetQtConceptMap())
    ;
    throw std::invalid_argument(msg.str());
  }
}

std::string ribi::cmap::GetText(const CommandCreateNewEdge& c) noexcept
{
  return c.GetText();
}

ribi::cmap::CommandCreateNewEdge * ribi::cmap::ParseCommandCreateNewEdge(
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
  return new CommandCreateNewEdge(qtconceptmap, t);
}

void ribi::cmap::CommandCreateNewEdge::Redo()
{
  CheckCanRedo(); //Throws if not

  //NONSENSE, just use 'from' and 'to'
  //Expects(CountSelectedQtEdges(GetScene(*this)) == 0);
  //Expects(CountSelectedQtNodes(GetScene(*this)) == 2);

  GetQtConceptMap().GetScene().clearFocus();

  assert(m_from);
  assert(m_to);
  assert(m_to != m_from);
  assert(m_from->scene());
  assert(m_to->scene());
  const auto qtnodes = { m_from, m_to };
  assert(qtnodes.size() == 2);
  assert(!IsOnEdge(*m_from));
  assert(!IsOnEdge(*m_to));
  //NONSENSE, just use 'from' and 'to'
  //assert(CountSelectedQtNodes(GetQtConceptMap()) == 2);
  //assert(CountSelectedQtEdges(GetQtConceptMap()) == 0);

  //Create the new QtEdge
  m_added_qtedge = new QtEdge(
    Edge(
      Node(
        Concept(m_text),
        NodeType::normal,
        (GetX(*m_from) + GetX(*m_to)) / 2.0,
        (GetY(*m_from) + GetY(*m_to)) / 2.0
      ),
      true //Arrow head
    ),
    m_from,
    m_to
  );

  //-----------------------
  // Using
  //-----------------------
  assert(AllHaveScene(nullptr));
  GetQtConceptMap().GetScene().addItem(m_added_qtedge);
  assert(AllHaveScene(&GetQtConceptMap().GetScene()));

  m_added_qtedge->setFocus();
  //m_added_qtedge->GetQtNode()->setFocus();
  SetSelectedness(true, *m_added_qtedge);

  m_added_qtedge->GetQtNode()->SetBrushFunction(
    GetQtNodeBrushFunction(
      GetQtConceptMap().GetMode()
    )
  );

  QtNode * const no_qtnode{nullptr};
  SetQtToolItemBuddy(GetQtConceptMap(), no_qtnode);

  SetSelectedness(false, *m_added_qtedge->GetFrom());
  SetSelectedness(false, *m_added_qtedge->GetTo());

  //Edges connected to the center node must be smaller
  if (IsConnectedToCenterNode(*m_added_qtedge))
  {
    m_added_qtedge->GetQtNode()->SetInnerWidth(8);
    m_added_qtedge->GetQtNode()->SetInnerHeight(8);
  }

  assert(m_added_qtedge->GetFrom()->scene());
  assert(m_added_qtedge->GetTo()->scene());
  Ensures(::ribi::cmap::GetText(*m_added_qtedge) == m_text);
}

void ribi::cmap::CommandCreateNewEdge::Undo()
{
  //NONSENSE, just use 'from' and 'to'
  //Expects(CountSelectedQtEdges(GetScene(*this)) == 1);
  //Expects(CountSelectedQtNodes(GetScene(*this)) == 0);

  GetQtConceptMap().GetScene().clearFocus();

  SetSelectedness(false, *m_added_qtedge);

  //QtConceptMap
  assert(AllHaveScene(&GetQtConceptMap().GetScene()));

  GetQtConceptMap().GetScene().removeItem(m_added_qtedge);
  assert(!m_added_qtedge->GetQtNode()->scene());
  assert(!m_added_qtedge->GetArrow()->scene());
  //GetQtConceptMap().GetScene().removeItem(m_added_qtnode); //Get these for free
  //GetQtConceptMap().GetScene().removeItem(m_added_qtedge->GetArrow()); //Get these for free  this->m_

  SetSelectedness(true, *m_added_qtedge->GetFrom());
  SetSelectedness(true, *m_added_qtedge->GetTo());
  m_added_qtedge->GetTo()->setFocus();

  //NONSENSE, just use 'from' and 'to'
  //Ensures(CountSelectedQtEdges(GetScene(*this)) == 0);
  //Ensures(CountSelectedQtNodes(GetScene(*this)) == 2);
}
