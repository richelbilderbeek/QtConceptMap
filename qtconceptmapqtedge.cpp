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
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QPainter>

#include "conceptmapconcept.h"
#include "conceptmapedge.h"
#include "conceptmapnode.h"
#include "container.h"
#include "qtconceptmaphelper.h"
#include "qtconceptmapbrushfactory.h"
#include "qtconceptmapqtnode.h"
#include "qtquadbezierarrowitem.h"

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

  this->setFlags(QGraphicsItem::ItemIsSelectable);
  //this->setFlags(0);

  GetQtNode()->SetContourPen(QPen(Qt::white));
  {
    //Cannot simplify this to the line below, for reasons unknown
    //GetQtNode()->SetFocusPen(QPen(Qt::black, Qt::DashLine));
    QPen focus_pen(Qt::black);
    focus_pen.setStyle(Qt::DashLine);
    GetQtNode()->SetFocusPen(focus_pen);
  }

  GetQtNode()->setFlags(GetQtNodeFlags());

  //m_edge must be initialized before m_arrow
  //if 'from' or 'to' are CenterNodes, then no item must be put at the center
  if (IsConnectedToCenterNode(*this))
  {
    m_arrow->SetMidX( (m_arrow->GetFromX() + m_arrow->GetToX()) / 2.0 );
    m_arrow->SetMidY( (m_arrow->GetFromY() + m_arrow->GetToY()) / 2.0 );
  }

  m_qtnode->SetCenterX(GetX(m_edge));
  m_qtnode->SetCenterY(GetY(m_edge));
  m_qtnode->SetText( { GetText(m_edge) } );

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

void ribi::cmap::CheckInvariants(const QtEdge& qtedge)
{
  QGraphicsScene * const scene = qtedge.scene();
  assert(scene);
  assert(scene == qtedge.GetFrom()->scene());
  assert(scene == qtedge.GetTo()->scene());
  assert(qtedge.GetArrow());
  assert(scene == qtedge.GetArrow()->scene());
  assert(qtedge.GetQtNode());
  assert(scene == qtedge.GetQtNode()->scene());
  assert(qtedge.GetQtNode()->GetContourPen().color() == Qt::white);
  assert(qtedge.GetArrow()->GetPen().color() == Qt::black);
  CheckInvariants(*qtedge.GetQtNode());
  assert(GetX(qtedge) == GetX(qtedge.GetEdge()));
  assert(GetX(qtedge) == GetX(*qtedge.GetQtNode()));

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

/*
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
*/

QGraphicsItem::GraphicsItemFlags ribi::cmap::GetQtNodeFlags() noexcept
{
  return QGraphicsItem::ItemIsFocusable
    | QGraphicsItem::ItemIsMovable
    | QGraphicsItem::ItemIsSelectable
  ;
}

std::string ribi::cmap::GetText(const QtEdge& qtedge) noexcept
{
  return GetText(qtedge.GetEdge());
}

double ribi::cmap::GetX(const QtEdge& qtedge) noexcept
{
  assert(std::abs(GetX(qtedge.GetEdge()) - GetX(*qtedge.GetQtNode())) < 2.0);
  return GetX(*qtedge.GetQtNode());
}

double ribi::cmap::GetY(const QtEdge& qtedge) noexcept
{
  assert(std::abs(GetY(qtedge.GetEdge()) - GetY(*qtedge.GetQtNode())) < 2.0);
  return GetY(*qtedge.GetQtNode());
}

bool ribi::cmap::HasExamples(const QtEdge& qtedge) noexcept
{
  assert(qtedge.GetQtNode());
  return HasExamples(*qtedge.GetQtNode());
}

bool ribi::cmap::HasHeadArrow(const QtEdge& qtedge) noexcept
{
  assert(qtedge.GetEdge().HasHeadArrow() == qtedge.GetArrow()->HasHead());
  return qtedge.GetArrow()->HasHead();
}

bool ribi::cmap::HasTailArrow(const QtEdge& qtedge) noexcept
{
  assert(qtedge.GetEdge().HasTailArrow() == qtedge.GetArrow()->HasTail());
  return qtedge.GetArrow()->HasTail();
}

bool ribi::cmap::IsConnectedToCenterNode(const QtEdge& qtedge)
{
  return IsQtCenterNode(qtedge.GetFrom())
    || IsQtCenterNode(qtedge.GetTo());
}

bool ribi::cmap::IsEnabled(const QtEdge& qtedge) noexcept
{
  return qtedge.isEnabled();
}

bool ribi::cmap::IsMovable(const QtEdge& qtedge) noexcept
{
  //A QtEdge is always immovable
  assert(!(qtedge.flags() & QGraphicsItem::ItemIsMovable));

  //Its QtNode (which it follows) can be moved
  return qtedge.GetQtNode()->flags() & QGraphicsItem::ItemIsMovable;
}

bool ribi::cmap::IsSelectable(const QtEdge& qtedge) noexcept
{
  return qtedge.flags() & QGraphicsItem::ItemIsSelectable;
}

bool ribi::cmap::IsSelected(const QtEdge& qtedge) noexcept
{
  return qtedge.IsSelected();
}

bool ribi::cmap::IsVisible(const QtEdge& qtedge) noexcept
{
  return qtedge.isVisible();
}

bool ribi::cmap::QtEdge::IsSelected() const
{
  assert(QGraphicsItem::isSelected() == this->GetQtNode()->isSelected()
    //If the QtEdge is connected to the center node, the QtNode is made invisible
    || IsConnectedToCenterNode(*this)
  );
  return QGraphicsItem::isSelected();
}

/*
void ribi::cmap::QtEdge::keyPressEvent(QKeyEvent *event) noexcept
{
  //Don't forward the keyPressEvent!
  //These are handled by Commands in the QtConceptMap
  //if (1 == 2)
  {
    //m_arrow->keyPressEvent(event);
    //m_edge.SetHeadArrow(m_arrow->HasHead());
    //m_edge.SetTailArrow(m_arrow->HasTail());
  }
  QGraphicsItem::keyPressEvent(event);
}
*/

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

void ribi::cmap::Move(QtEdge& qtedge, const double dx, const double dy)
{
  assert(GetX(qtedge) == GetX(*qtedge.GetQtNode()));
  assert(GetX(qtedge) == GetX( qtedge.GetQtNode()->GetNode()));
  assert(GetX(qtedge) == GetX( qtedge.GetEdge()));
  assert(GetX(qtedge) == GetX( qtedge.GetEdge().GetNode()));

  Move(*qtedge.GetQtNode(), dx, dy);
  Move(qtedge.GetEdge(), dx, dy);

  assert(GetX(qtedge) == GetX(*qtedge.GetQtNode()));
  assert(GetX(qtedge) == GetX( qtedge.GetQtNode()->GetNode()));
  assert(GetX(qtedge) == GetX( qtedge.GetEdge()));
  assert(GetX(qtedge) == GetX( qtedge.GetEdge().GetNode()));
}

void ribi::cmap::QtEdge::paint(
  QPainter* painter,
  const QStyleOptionGraphicsItem* /*option*/,
  QWidget* /*widget*/
) noexcept
{
  CheckInvariants(*this);
  {
    const QPen pen{
      this->hasFocus() || this->IsSelected()
      ? QPen(Qt::black)
      : QPen(Qt::black)
    };
    m_arrow->SetPen(pen);
  }
  {
    const QPen pen{
      this->hasFocus() || this->IsSelected()
      ? QPen(Qt::white)
      : QPen(Qt::white)
    };
    m_qtnode->setPen(pen);
  }
  ShowBoundingRect(painter);
  CheckInvariants(*this);
}

void ribi::cmap::QtEdge::SetEdge(const Edge& edge) noexcept
{
  m_edge = edge;

  m_qtnode->SetCenterX(GetX(m_edge));
  m_qtnode->SetCenterY(GetY(m_edge));
  m_qtnode->SetText( { GetText(m_edge) } );
  SetHasHeadArrow(m_edge.HasHeadArrow());
  SetHasTailArrow(m_edge.HasTailArrow());
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

void ribi::cmap::QtEdge::SetSelected(const bool selected)
{
  QGraphicsItem::setSelected(selected);
  this->GetQtNode()->setSelected(selected);
  assert(GetQtNode()->isSelected() == QGraphicsItem::isSelected()
    || IsConnectedToCenterNode(*this)
  );
  assert(IsSelected() == selected);
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

void ribi::cmap::QtEdge::ShowBoundingRect(QPainter* const painter)
{
  if (m_show_bounding_rect)
  {
    assert(painter);
    const QPen prev_pen = painter->pen();
    const QBrush prev_brush = painter->brush();
    painter->setPen(QPen(QColor(96,0,0)));
    painter->setBrush(QBrush(QColor(255,0,0,32)));
    painter->drawRect(this->boundingRect().adjusted(1.0,1.0,-1.0,-1.0));
    painter->setPen(prev_pen);
    painter->setBrush(prev_brush);
  }
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

bool ribi::cmap::operator==(const QtEdge& lhs, const QtEdge& rhs) noexcept
{
  return
       lhs.GetArrow() == rhs.GetArrow()
    && lhs.GetEdge() == rhs.GetEdge()
    && lhs.GetFrom() == rhs.GetFrom()
    && lhs.GetQtNode() == rhs.GetQtNode()
    && lhs.GetTo() == rhs.GetTo()
  ;
}
