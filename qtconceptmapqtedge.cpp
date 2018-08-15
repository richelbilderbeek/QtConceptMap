#include "qtconceptmapqtedge.h"

#include <cassert>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>

#include "qtconceptmap.h"
#include "qtconceptmaphelper.h"
#include "qtconceptmapqtnode.h"
#include "qtquadbezierarrowitem.h"

ribi::cmap::QtEdge::QtEdge(
    const Edge& edge,
    QtNode * const from,
    QtNode * const to
) : QtEdge(
  edge.GetNode(),
  from,
  to,
  edge.HasTailArrow(),
  edge.HasHeadArrow(),
  edge.GetId()
)
{
  assert(std::abs(GetX(*this) - GetX(edge)) < 2.0);
  assert(std::abs(GetY(*this) - GetY(edge)) < 2.0);
}

ribi::cmap::QtEdge::QtEdge(
    const Node& node,
    QtNode * const from,
    QtNode * const to,
    const bool has_tail_arrow,
    const bool has_head_arrow,
    const int edge_id
) : QtEdge(
  node.GetConcept(),
  node.GetX(),
  node.GetY(),
  from,
  to,
  has_tail_arrow,
  has_head_arrow,
  edge_id
)
{
  #ifndef NDEBUG
  if (std::abs(GetX(*this) - GetX(node)) >= 2.0)
  {
    qCritical()
      << GetX(*this)
      << GetX(node)
    ;
  }
  #endif
  assert(std::abs(GetX(*this) - GetX(node)) < 2.0);
  assert(std::abs(GetY(*this) - GetY(node)) < 2.0);

}

ribi::cmap::QtEdge::QtEdge(
    const Concept& concept,
    const double x,
    const double y,
    QtNode * const from,
    QtNode * const to,
    const bool has_tail_arrow,
    const bool has_head_arrow,
    const int edge_id
)
  : m_arrow{nullptr}, //Will be initialized below
    m_from{from},
    m_id{edge_id},
    m_qtnode{
      new QtNode(
        concept,
        Node().GetId(), //Nodes have a static counter for IDs
        NodeType::normal,
        x,
        y,
        this
      )
    }, //parent
    m_show_bounding_rect{false},
    m_to{to}
{
  assert(IsOnEdge(*m_qtnode));
  CheckInput(from, to);
  m_arrow = new QtQuadBezierArrowItem(
    from,
    has_tail_arrow,
    this->GetQtNode(),
    has_head_arrow,
    to,
    this // parent
  );

  this->m_arrow->setFlags(0);

  //QtEdge must be selectable
  this->setFlags(QGraphicsItem::ItemIsSelectable);

  GetQtNode()->SetContourPen(QPen(Qt::white));

  {
    //Cannot simplify this to the line below, for reasons unknown
    //GetQtNode()->SetFocusPen(QPen(Qt::black, Qt::DashLine));
    QPen focus_pen(Qt::black);
    focus_pen.setStyle(Qt::DashLine);
    GetQtNode()->SetFocusPen(focus_pen);
  }

  GetQtNode()->setFlags(CreateEditFlags(*GetQtNode()));

  #ifdef STRAIGHT_LINES_BETWEEN_CENTER_NODE_AND_PRIMARY_CONCEPTS
  //m_edge must be initialized before m_arrow
  //if 'from' or 'to' are CenterNodes, then no item must be put at the center
  if (IsConnectedToCenterNode(*this))
  {
    m_arrow->SetMidX( (m_arrow->GetFromX() + m_arrow->GetToX()) / 2.0 );
    m_arrow->SetMidY( (m_arrow->GetFromY() + m_arrow->GetToY()) / 2.0 );
  }
  #endif // STRAIGHT_LINES_BETWEEN_CENTER_NODE_AND_PRIMARY_CONCEPTS

  m_qtnode->SetCenterPos(x, y);

  //Set Z values
  this->setZValue(GetQtEdgeZvalue());
  m_arrow->setZValue(GetQtEdgeArrowZvalue());
  m_qtnode->setZValue(GetQtNodeZvalue());
}

ribi::cmap::QtEdge::~QtEdge() noexcept
{

}

QRectF ribi::cmap::QtEdge::boundingRect() const
{
  //QMarginsF ruthlessly prevents ugly stains
  return m_qtnode->boundingRect().translated(m_qtnode->pos())
    .united(m_arrow->boundingRect())
    .adjusted(-300.0, -300.0, 300.0, 300.0)
  ;

  //Don't forget to update ::shape if you change ::boundingRect!

  //Some other candidates

  //return m_qtnode->boundingRect()
  //  .united(m_arrow->boundingRect());

  //return m_qtnode->boundingRect().translated(-m_qtnode->pos())
  //   .united(m_arrow->boundingRect());

  //return m_qtnode->boundingRect()
  //  .united(m_arrow->boundingRect().translated(m_qtnode->pos()));
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
  if (IsOnEdge(*from))
  {
    throw std::invalid_argument("QtNode 'from' must not be on an edge");
  }
  if (IsOnEdge(*to))
  {
    throw std::invalid_argument("QtNode 'from' must not be on an edge");
  }
}

void ribi::cmap::CheckInvariants(const QtEdge& qtedge) //!OCLINT cannot make this simpler
{
  if (QGraphicsScene * const scene = qtedge.scene())
  {
    assert(scene == qtedge.GetFrom()->scene());
    assert(scene == qtedge.GetTo()->scene());
    assert(scene == qtedge.GetArrow()->scene());
    assert(scene == qtedge.GetQtNode()->scene());
  }
  assert(qtedge.GetArrow());
  assert(qtedge.GetArrow()->GetPen().color() == Qt::black);
  assert(qtedge.GetQtNode());
  assert(qtedge.GetQtNode()->GetContourPen().color() == Qt::white);
  assert(GetX(qtedge) == GetX(*qtedge.GetQtNode()));
  assert(GetY(qtedge) == GetY(*qtedge.GetQtNode()));
  assert(qtedge.zValue() == GetQtEdgeZvalue());
  assert(qtedge.GetArrow()->zValue() == GetQtEdgeArrowZvalue());
  assert(qtedge.GetQtNode()->zValue() == GetQtNodeZvalue());
}

void ribi::cmap::QtEdge::dragEnterEvent(QGraphicsSceneDragDropEvent * event)
{
  assert(!"ribi::cmap::QtEdge::dragEnterEvent is never called"); //!OCLINT accepted idiom
  QGraphicsItem::dragEnterEvent(event);
}

QPointF ribi::cmap::GetCenterPos(const QtEdge& qtedge) noexcept
{
  return GetCenterPos(*qtedge.GetQtNode());
}

ribi::cmap::Concept ribi::cmap::GetConcept(const QtEdge& qtedge) noexcept
{
  return GetConcept(*qtedge.GetQtNode());
}

ribi::cmap::Edge ribi::cmap::QtEdge::GetEdge() const noexcept
{
  return Edge(
    GetNode(*m_qtnode),
    m_arrow->HasHead(),
    m_arrow->HasTail(),
    GetId()
  );
}

ribi::cmap::Node ribi::cmap::GetNode(const QtEdge& qtedge) noexcept
{
  return ::ribi::cmap::GetNode(*qtedge.GetQtNode());
}

std::string ribi::cmap::GetText(const QtEdge& qtedge) noexcept
{
  return GetText(*qtedge.GetQtNode());
}

double ribi::cmap::GetX(const QtEdge& qtedge) noexcept
{
  return GetX(*qtedge.GetQtNode());
}

double ribi::cmap::GetY(const QtEdge& qtedge) noexcept
{
  return GetY(*qtedge.GetQtNode());
}

bool ribi::cmap::HasExamples(const QtEdge& qtedge) noexcept
{
  return HasExamples(*qtedge.GetQtNode());
}

bool ribi::cmap::HasHeadArrow(const QtEdge& qtedge) noexcept
{
  return qtedge.GetArrow()->HasHead();
}

bool ribi::cmap::HasTailArrow(const QtEdge& qtedge) noexcept
{
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
  return QGraphicsItem::isSelected();
}

/*
void ribi::cmap::QtEdge::keyPressEvent(QKeyEvent *event) noexcept
{
  //Don't forward the keyPressEvent!
  //These are handled by Commands in the QtConceptMap
}
*/

void ribi::cmap::QtEdge::mousePressEvent(QGraphicsSceneMouseEvent *event) noexcept
{  
  if (event->modifiers() & Qt::ShiftModifier)
  {
    if ((event->pos() - this->m_arrow->GetTail()
      + m_qtnode->pos()).manhattanLength() < 20.0)
    {
      this->SetHasTailArrow( !m_arrow->HasTail() );
      //this->update(); //Don't!
      return;
    }
    if ((event->pos() - this->m_arrow->GetHead()
      + m_qtnode->pos()).manhattanLength() < 20.0)
    {
      this->SetHasHeadArrow( !m_arrow->HasHead() );
      //this->update(); //Don't!
      return;
    }
  }

  //What is clicked on: the concept or the arrow? Assume concept
  m_arrow->SetPen(QPen(QColor(0,0,0)));
  if (!m_qtnode->GetInnerRect().contains(event->pos()))
  {
    //If the concept is not clicked...
    //but the arrow is...
    QPointF pos_on_arrow = event->pos();
    pos_on_arrow += (m_qtnode->pos());
    if (m_arrow->shape().contains(pos_on_arrow)
      || (event->pos() - this->m_arrow->GetTail()
        + m_qtnode->pos()).manhattanLength() < 20.0
      || (event->pos() - this->m_arrow->GetHead()
        + m_qtnode->pos()).manhattanLength() < 20.0
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

  Move(*qtedge.GetQtNode(), dx, dy);

  assert(GetX(qtedge) == GetX(*qtedge.GetQtNode()));
}

void ribi::cmap::QtEdge::paint(
  QPainter* painter,
  const QStyleOptionGraphicsItem* /*option*/,
  QWidget* /*widget*/
) noexcept
{
  CheckInvariants(*this);

  #ifdef STRAIGHT_LINES_BETWEEN_CENTER_NODE_AND_PRIMARY_CONCEPTS
  //When connecting to center node, the center of the arrow must
  //be kept between source and target node
  if (IsConnectedToCenterNode(*this))
  {
    const double x_in_middle{(m_arrow->GetFromX() + m_arrow->GetToX()) / 2.0};
    const double y_in_middle{(m_arrow->GetFromY() + m_arrow->GetToY()) / 2.0};
    m_qtnode->SetCenterX(x_in_middle);
    m_qtnode->SetCenterY(y_in_middle);
    m_arrow->SetMidX(x_in_middle);
    m_arrow->SetMidY(y_in_middle);
  }
  #endif // STRAIGHT_LINES_BETWEEN_CENTER_NODE_AND_PRIMARY_CONCEPTS

  {
    QPen pen(Qt::black);
    assert(!IsDashed(pen));
    if (this->hasFocus() || this->IsSelected())
    {
      pen.setStyle(Qt::DashLine);
      assert(IsDashed(pen));
    }

    m_arrow->SetPen(pen);
  }
  {
    QPen pen(Qt::white);
    assert(!IsDashed(pen));
    if (this->hasFocus() || this->IsSelected())
    {
      pen.setStyle(Qt::DashLine);
      assert(IsDashed(pen));
    }
    m_qtnode->setPen(pen);
  }
  ShowBoundingRect(painter);
  CheckInvariants(*this);
}

std::function<bool(ribi::cmap::QtEdge* const)>
  ribi::cmap::QtEdgeHasName(const std::string& name)
{
  return [name](const QtEdge * const qtedge)
  {
    assert(qtedge);
    return GetText(*qtedge) == name;
  };
}

void ribi::cmap::SetConcept(QtEdge& qtedge, const Concept& concept) noexcept
{
  SetConcept(*qtedge.GetQtNode(), concept);
}

void ribi::cmap::SetExamples(
  QtEdge& qtedge,
  const Examples& examples) noexcept
{
  assert(qtedge.GetQtNode());
  SetExamples(*qtedge.GetQtNode(), examples);
}

void ribi::cmap::QtEdge::SetHasHeadArrow(const bool has_head_arrow) noexcept
{
  assert(m_arrow);
  this->m_arrow->SetHasHead(has_head_arrow);
}

void ribi::cmap::QtEdge::SetHasTailArrow(const bool has_tail_arrow) noexcept
{
  assert(m_arrow);
  this->m_arrow->SetHasTail(has_tail_arrow);
}

void ribi::cmap::SetIsComplex(QtEdge& qtedge, const bool is_complex)
{
  SetIsComplex(*qtedge.GetQtNode(), is_complex);
}

void ribi::cmap::QtEdge::SetSelected(const bool selected)
{
  QGraphicsItem::setSelected(selected);
  this->GetQtNode()->setSelected(selected);
}

QPainterPath ribi::cmap::QtEdge::shape() const noexcept
{
  assert(m_qtnode);
  assert(m_arrow);
  return m_qtnode->shape().translated(m_qtnode->pos())
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
    << (*qtedge.GetQtNode())
  ;
  return os;
}

bool ribi::cmap::operator==(const QtEdge& lhs, const QtEdge& rhs) noexcept
{
  return
       lhs.GetArrow() == rhs.GetArrow()
    && lhs.GetFrom() == rhs.GetFrom()
    && lhs.GetQtNode() == rhs.GetQtNode()
    && lhs.GetTo() == rhs.GetTo()
  ;
}
