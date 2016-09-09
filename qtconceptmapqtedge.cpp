//---------------------------------------------------------------------------
/*
QtConceptMap, Qt classes for display and interaction with ConceptMap
Copyright (C) 2013-2016 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/CppQtConceptMap.htm
//---------------------------------------------------------------------------
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "qtconceptmapqtedge.h"

#include <cassert>
#include <cmath>
#include <boost/lambda/lambda.hpp>

#include <QCursor>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QPainter>

#include "conceptmapconcept.h"
#include "conceptmapedge.h"
#include "conceptmapnode.h"
#include "container.h"
#include "qtconceptmapbrushfactory.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmapcenternode.h"
#include "qtquadbezierarrowitem.h"
#include "trace.h"
#pragma GCC diagnostic pop

ribi::cmap::QtEdge::QtEdge(
    const Edge& edge,
    QtNode * const from,
    QtNode * const to
)
  : m_arrow{nullptr}, //Will be initialized below
    m_edge{edge},
    m_from{from},
    m_qtnode{new QtNode(edge.GetNode(), this)}, //parent
    m_show_bounding_rect{false},
    m_to{to}
{
  CheckInput(from, to);
  m_arrow = new QtQuadBezierArrowItem(
    from,
    false, //edge.HasTailArrow(),
    this->GetQtNode(),
    false, //edge.HasHeadArrow(),
    to,
    this // parent
  );

  this->m_arrow->setFlags(0);

  this->setFlags(0);

  GetQtNode()->setFlags(GetQtNodeFlags());

  //m_edge must be initialized before m_arrow
  //if 'from' or 'to' are CenterNodes, then no item must be put at the center
  const bool is_connected_to_center_node
    = dynamic_cast<QtCenterNode*>(from) || dynamic_cast<QtCenterNode*>(to);
  if (is_connected_to_center_node)
  {
    m_arrow->SetMidX( (m_arrow->GetFromX() + m_arrow->GetToX()) / 2.0 );
    m_arrow->SetMidY( (m_arrow->GetFromY() + m_arrow->GetToY()) / 2.0 );
  }

  m_edge.GetNode().SetX( (from->GetCenterX() + to->GetCenterX()) / 2.0 );
  m_edge.GetNode().SetY( (from->GetCenterY() + to->GetCenterY()) / 2.0 );

  m_qtnode->SetCenterX(m_edge.GetNode().GetX());
  m_qtnode->SetCenterY(m_edge.GetNode().GetY());
  m_qtnode->SetText( { m_edge.GetNode().GetConcept().GetName() } );
  assert(std::abs(m_edge.GetNode().GetY() - m_qtnode->GetCenterY()) < 2.0);

  this->SetEdge(m_edge);

  //Set Z values
  this->setZValue(-1.0);
  m_arrow->setZValue(-1.0);
  m_qtnode->setZValue(1.0);


}

ribi::cmap::QtEdge::~QtEdge() noexcept
{

}

QRectF ribi::cmap::QtEdge::boundingRect() const
{
  assert(this->scene());
  assert(m_qtnode->scene());
  assert(m_arrow->scene());
  return m_qtnode->boundingRect().translated(m_qtnode->GetCenterPos())
    .united(m_arrow->boundingRect())
  ;

  //Don't forget to update ::shape if you change ::boundingRect!

  //Some other candidates

  //return m_qtnode->boundingRect()
  //  .united(m_arrow->boundingRect());

  //return m_qtnode->boundingRect().translated(-m_qtnode->GetCenterPos())
  //   .united(m_arrow->boundingRect());

  //return m_qtnode->boundingRect()
  //  .united(m_arrow->boundingRect().translated(m_qtnode->GetCenterPos()));
}

void ribi::cmap::QtEdge::CheckInput(QtNode * const from, QtNode * const to)
{
  if (!from)
  {
    throw std::invalid_argument("QtEdge must have a non-nullptr from");
  }
  if (!to)
  {
    throw std::invalid_argument("QtEdge must have a non-nullptr to");
  }
  if (from == to)
  {
    throw std::invalid_argument("QtEdge must have a different from and to");
  }
}

void ribi::cmap::DisableAll(QtEdge& qtedge) noexcept
{
  qtedge.setEnabled(false);
  qtedge.setVisible(false);
  qtedge.GetArrow()->setEnabled(false);
  qtedge.GetArrow()->setVisible(false);
}

void ribi::cmap::EnableAll(QtEdge& qtedge) noexcept
{
  qtedge.setEnabled(true);
  qtedge.setVisible(true);
  qtedge.GetArrow()->setEnabled(true);
  qtedge.GetArrow()->setVisible(true);
}

void ribi::cmap::QtEdge::focusInEvent(QFocusEvent* e) noexcept
{
  QGraphicsItem::focusInEvent(e);
  assert(hasFocus());
}

void ribi::cmap::QtEdge::focusOutEvent(QFocusEvent* e) noexcept
{
  QGraphicsItem::focusOutEvent(e);
  assert(!hasFocus());
}

QGraphicsItem::GraphicsItemFlags ribi::cmap::GetQtNodeFlags() noexcept
{
  return QGraphicsItem::ItemIsFocusable
    | QGraphicsItem::ItemIsMovable
    | QGraphicsItem::ItemIsSelectable
  ;
}

bool ribi::cmap::QtEdge::HasHeadArrow() const noexcept
{
  assert(m_arrow);
  assert(GetEdge().HasHeadArrow() == m_arrow->HasHead());
  return m_arrow->HasHead();
}

bool ribi::cmap::QtEdge::HasTailArrow() const noexcept
{
  assert(m_arrow);
  assert(GetEdge().HasTailArrow() == m_arrow->HasTail());
  return m_arrow->HasTail();
}

bool ribi::cmap::QtEdge::isSelected() const
{
  return GetQtNode()->isSelected();
}

void ribi::cmap::QtEdge::keyPressEvent(QKeyEvent *event) noexcept
{
  //Don't forward the keyPressEvent!
  //These are handled by Commands in the QtConceptMap
  if (1 == 2)
  {
    assert(m_arrow);
    m_arrow->keyPressEvent(event);
    m_edge.SetHeadArrow(m_arrow->HasHead());
    m_edge.SetTailArrow(m_arrow->HasTail());
    assert(m_edge.HasHeadArrow() == m_arrow->HasHead());
    assert(m_edge.HasTailArrow() == m_arrow->HasTail());
  }
  QGraphicsItem::keyPressEvent(event);
}

void ribi::cmap::QtEdge::mousePressEvent(QGraphicsSceneMouseEvent *event) noexcept
{  
  if (event->modifiers() & Qt::ShiftModifier)
  {
    if ((event->pos() - this->m_arrow->GetTail()
      + m_qtnode->GetCenterPos()).manhattanLength() < 20.0)
    {
      this->SetHasTailArrow( !m_arrow->HasTail() );
      //this->update(); //Don't!
    }
    else if ((event->pos() - this->m_arrow->GetHead()
      + m_qtnode->GetCenterPos()).manhattanLength() < 20.0)
    {
      this->SetHasHeadArrow( !m_arrow->HasHead() );
      //this->update(); //Don't!
    }
  }

  //What is clicked on: the concept or the arrow? Assume concept
  m_arrow->SetPen(QPen(QColor(0,0,0)));
  if (!m_qtnode->GetInnerRect().contains(event->pos()))
  {
    //If the concept is not clicked...
    //but the arrow is...
    QPointF pos_on_arrow = event->pos();
    pos_on_arrow += (m_qtnode->GetCenterPos());
    if (m_arrow->shape().contains(pos_on_arrow)
      || (event->pos() - this->m_arrow->GetTail()
        + m_qtnode->GetCenterPos()).manhattanLength() < 20.0
      || (event->pos() - this->m_arrow->GetHead()
        + m_qtnode->GetCenterPos()).manhattanLength() < 20.0
      )
    {
      //give focus to the arrow
      m_arrow->SetPen(m_arrow->GetFocusPen());
      return;
    }
  }
  //QtConceptMapElement::mousePressEvent(event);
}

void ribi::cmap::QtEdge::paint(
  QPainter* painter,
  const QStyleOptionGraphicsItem* /*option*/,
  QWidget* /*widget*/
) noexcept
{
  if (!this->scene())
  {
    qCritical() << "BREAK";
  }
  assert(this->scene());
  assert(this->m_from->scene());
  assert(this->m_to->scene());
  assert(this->scene() == m_from->scene());
  assert(this->scene() == m_to->scene());

  assert(m_arrow);
  assert(this->m_arrow->scene());
  assert(this->scene() == this->m_arrow->scene());

  assert(m_qtnode);
  assert(this->m_qtnode->scene());
  assert(this->scene() == this->m_qtnode->scene());

  const QPen pen{
    this->hasFocus() || this->isSelected()
    ? this->GetQtNode()->GetFocusPen()
    : this->GetQtNode()->GetContourPen()
  };
  m_arrow->SetPen(pen);
  m_qtnode->setPen(pen);

  if (m_show_bounding_rect)
  {
    const QPen prev_pen = painter->pen();
    const QBrush prev_brush = painter->brush();
    painter->setPen(QPen(QColor(96,0,0)));
    painter->setBrush(QBrush(QColor(255,0,0,32)));
    painter->drawRect(this->boundingRect().adjusted(1.0,1.0,-1.0,-1.0));
    painter->setPen(prev_pen);
    painter->setBrush(prev_brush);
  }
}

void ribi::cmap::QtEdge::SetEdge(const Edge& edge) noexcept
{
  m_edge = edge;

  //Sync
  m_qtnode->SetCenterX(m_edge.GetNode().GetX());
  m_qtnode->SetCenterY(m_edge.GetNode().GetY());
  m_qtnode->SetText( { m_edge.GetNode().GetConcept().GetName() } );
  SetHasHeadArrow(m_edge.HasHeadArrow());
  SetHasTailArrow(m_edge.HasTailArrow());

  assert(edge == m_edge);
}

void ribi::cmap::QtEdge::SetHasHeadArrow(const bool has_head_arrow) noexcept
{
  assert(m_arrow);
  this->GetEdge().SetHeadArrow(has_head_arrow);
  this->m_arrow->SetHasHead(has_head_arrow);
}

void ribi::cmap::QtEdge::SetHasTailArrow(const bool has_tail_arrow) noexcept
{
  assert(m_arrow);
  this->GetEdge().SetTailArrow(has_tail_arrow);
  this->m_arrow->SetHasTail(has_tail_arrow);
}

void ribi::cmap::QtEdge::SetSelected(bool selected)
{
  this->GetQtNode()->setSelected(selected);
}

QPainterPath ribi::cmap::QtEdge::shape() const noexcept
{
  assert(this->scene());
  assert(m_qtnode->scene());
  assert(m_arrow->scene());
  assert(m_qtnode);
  assert(m_arrow);
  return m_qtnode->shape().translated(m_qtnode->GetCenterPos())
    .united(m_arrow->shape())
  ;
}

std::string ribi::cmap::ToStr(const QtEdge& qtedge) noexcept
{
  std::stringstream s;
  s << qtedge;
  return s.str();
}

std::ostream& ribi::cmap::operator<<(std::ostream& os, const QtEdge& qtedge) noexcept
{
  os
    << (qtedge.GetEdge())
  ;
  return os;
}

bool ribi::cmap::operator==(const QtEdge& /*lhs*/, const QtEdge& /*rhs*/) noexcept
{
  //A stub
  return true;
}
