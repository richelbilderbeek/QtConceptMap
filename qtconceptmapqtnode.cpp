#include "qtconceptmapqtnode.h"

#include <cassert>
#include <climits>

#include <QCursor>
#include <QDebug>
#include <QGraphicsItem>
#include <QKeyEvent>
#include <QPainter>
#include <QPen>

#include <gsl/gsl_assert>

#include "conceptmapconcept.h"
#include "conceptmapexamples.h"
#include "conceptmaphelper.h"
#include "conceptmapnodefactory.h"
#include "conceptmapnode.h"
#include "container.h"
#include "qtconceptmapbrushfactory.h"
#include "qtconceptmaphelper.h"
#include "qtconceptmapqtnodefactory.h"


ribi::cmap::QtNode::QtNode(
  const Node& node,
  QGraphicsItem* parent
) : QtNode(
      node.GetConcept(),
      node.GetId(),
      node.IsCenterNode(),
      node.GetX(),
      node.GetY(),
      parent
    )
{
  assert(m_id == node.GetId());
}

ribi::cmap::QtNode::QtNode(
  const Concept& concept,
  const int id,
  const bool is_center_node,
  const double center_x,
  const double center_y,
  QGraphicsItem* parent)
  : QtRoundedEditRectItem(
      { concept.GetName() },
      ribi::QtRoundedEditRectItem::Padding(),
      QFont("monospace",9),
      parent
    ),
    m_brush_function{GetQtNodeBrushFunctionUninitialized()},
    m_examples{concept.GetExamples()},
    m_id{id},
    m_is_complex{IsComplex(concept)},
    m_rating_complexity{concept.GetRatingComplexity()},
    m_rating_concreteness{concept.GetRatingConcreteness()},
    m_rating_specificity{concept.GetRatingSpecificity()},
    m_show_bounding_rect{false}
{
  assert(m_id == id);
  //Allow mouse tracking
  this->setAcceptHoverEvents(true);

  this->SetPadding(QtRoundedEditRectItem::Padding(1.0,6.0,1.0,2.0));

  this->setAcceptHoverEvents(true);
  this->setZValue(0.0);
  this->SetContourPen(QPen(Qt::black, 1.0));
  this->SetFocusPen(QPen(Qt::black, 1.0, Qt::DashLine));
  SetNode(concept, is_center_node, center_x, center_y);
  CheckInvariants(*this);
  assert(m_id == id);
}

ribi::cmap::QtNode::~QtNode() noexcept
{

}

void ribi::cmap::CheckInvariants(const QtNode& qtnode) noexcept
{
  #ifndef NDEBUG
  const double x1{GetX(qtnode)};
  const double x2{qtnode.GetCenterX()};
  if ( std::abs(x1 - x2))
  {
    qCritical()
      << "\nx1: " << x1
      << "\nx2: " << x2
      << "\nGetText(qtnode) : " << GetText(qtnode).c_str()
    ;
  }
  assert(std::abs(x1 - x2) < 1.0);
  const double y1{GetY(qtnode)};
  const double y2{qtnode.GetCenterY()};
  if ( std::abs(y1 - y2))
  {
    qDebug()
      << "\ny1: " << y1
      << "\ny2: " << y2
      << "\nGetText(qtnode) : " << GetText(qtnode).c_str()
    ;
  }
  assert(std::abs(y1 - y2) < 1.0);
  #endif
}

void ribi::cmap::QtNode::DisableAll()
{
  this->setEnabled(false);
  this->setVisible(false);
}

void ribi::cmap::QtNode::EnableAll()
{
  this->setEnabled(true);
  this->setVisible(true);
}

void ribi::cmap::QtNode::focusInEvent(QFocusEvent* e) noexcept
{
  QtRoundedEditRectItem::focusInEvent(e);
  assert(hasFocus());
}

void ribi::cmap::QtNode::focusOutEvent(QFocusEvent* e) noexcept
{
  QtRoundedEditRectItem::focusOutEvent(e);
  assert(!hasFocus());
}

QPointF ribi::cmap::GetCenterPos(const QtNode& qtnode) noexcept
{
  return QPointF(
    qtnode.GetCenterX(),
    qtnode.GetCenterY()
  );
}

ribi::cmap::Concept ribi::cmap::GetConcept(const QtNode& qtnode) noexcept
{
  return Concept(
    GetName(qtnode),
    GetExamples(qtnode),
    IsComplex(qtnode),
    GetRatingComplexity(qtnode),
    GetRatingConcreteness(qtnode),
    GetRatingSpecificity(qtnode)
  );
}

const ribi::cmap::Examples& ribi::cmap::GetExamples(const QtNode& qtnode) noexcept
{
  return qtnode.GetExamples();
}

int ribi::cmap::GetRatingComplexity(const QtNode& qtnode) noexcept
{
  return GetRatingComplexity(GetNode(qtnode));
}

int ribi::cmap::GetRatingConcreteness(const QtNode& qtnode) noexcept
{
  return GetRatingConcreteness(GetNode(qtnode));
}

int ribi::cmap::GetRatingSpecificity(const QtNode& qtnode) noexcept
{
  return GetRatingSpecificity(GetNode(qtnode));
}

int ribi::cmap::GetId(const QtNode& qtnode) noexcept
{
  return qtnode.GetId();
}

std::string ribi::cmap::GetName(const QtNode& qtnode) noexcept
{
  return GetText(qtnode);
}

ribi::cmap::Node ribi::cmap::GetNode(const QtNode& qtnode) noexcept
{
  return Node(
    GetConcept(qtnode),
    IsCenterNode(qtnode),
    GetX(qtnode),
    GetY(qtnode)
  );
}

std::string ribi::cmap::GetText(const QtNode& qtnode) noexcept
{
  return Unwordwrap(qtnode.GetText());
}

double ribi::cmap::GetX(const QtNode& qtnode) noexcept
{
  return qtnode.GetCenterX();
}

double ribi::cmap::GetY(const QtNode& qtnode) noexcept
{
  return qtnode.GetCenterY();
}

bool ribi::cmap::HasExamples(const QtNode& qtnode) noexcept
{
  return !IsEmpty(qtnode.GetExamples());
}

void ribi::cmap::QtNode::hoverMoveEvent(QGraphicsSceneHoverEvent*) noexcept
{
  this->setCursor(QCursor(Qt::PointingHandCursor));
  //m_concept_item->hoverMoveEvent(e);
  //Won't cause a hover, because the concept item
  //is not visible??
}

bool ribi::cmap::IsCenterNode(const QtNode& qtnode) noexcept
{
  return !(qtnode.flags() & QGraphicsItem::ItemIsMovable);
}

bool ribi::cmap::IsComplex(const QtNode& qtnode) noexcept
{
  return qtnode.GetIsComplex();
}

bool ribi::cmap::IsEnabled(const QtNode& qtnode) noexcept
{
  return qtnode.isEnabled();
}

bool ribi::cmap::IsMovable(const QtNode& qtnode) noexcept
{
  return qtnode.flags() & QGraphicsItem::ItemIsMovable;
}

bool ribi::cmap::IsSelectable(const QtNode& qtnode) noexcept
{
  return qtnode.flags() & QGraphicsItem::ItemIsSelectable;
}

bool ribi::cmap::IsSelected(const QtNode& qtnode) noexcept
{
  return qtnode.isSelected();
}

bool ribi::cmap::IsVisible(const QtNode& qtnode) noexcept
{
  return qtnode.isVisible();
}

void ribi::cmap::QtNode::keyPressEvent(QKeyEvent *event) noexcept
{
  CheckInvariants(*this);

  QtRoundedEditRectItem::keyPressEvent(event);

  CheckInvariants(*this);
}

void ribi::cmap::Move(QtNode& qtnode, const double dx, const double dy)
{
  CheckInvariants(qtnode);

  qtnode.moveBy(dx, dy);

  CheckInvariants(qtnode);
}

void ribi::cmap::QtNode::paint(
  QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget
) noexcept
{
  CheckInvariants(*this);
  assert(this->scene());
  assert(painter);

  //Set the brush of this QtNode
  this->setBrush(m_brush_function(*this));

  QtRoundedEditRectItem::paint(painter,item,widget);

  if (HasExamples(*this))
  {
    painter->setBrush(Qt::transparent);
    painter->setPen(Qt::black);
    painter->drawRect(
      (GetInnerRect().width() / 2.0) - 5.0,
      -(GetInnerRect().height() / 2.0) + 3.0,
      3.0,
      3.0
    );
  }

  if (m_show_bounding_rect)
  {
    const QPen prev_pen = painter->pen();
    const QBrush prev_brush = painter->brush();
    painter->setPen(QPen(QColor(0,0,96)));
    painter->setBrush(QBrush(QColor(0,0,255,64)));
    painter->drawRect(this->boundingRect().adjusted(1.0,1.0,-1.0,-1.0));
    painter->setPen(prev_pen);
    painter->setBrush(prev_brush);
  }

  CheckInvariants(*this);
}

std::function<bool(const ribi::cmap::QtNode* const)>
  ribi::cmap::QtNodeHasId(const int id)
{
  return [id](const QtNode * const qtnode)
  {
    assert(qtnode);
    return GetId(*qtnode) == id;
  };
}

std::function<bool(const ribi::cmap::QtNode* const)>
  ribi::cmap::QtNodeHasName(const std::string& name)
{
  return [name](const QtNode * const qtnode)
  {
    assert(qtnode);
    return GetText(*qtnode) == name;
  };
}

void ribi::cmap::QtNode::SetBrushFunction(
  const std::function<QBrush(const ribi::cmap::QtNode&)>& f
) noexcept
{
  m_brush_function = f;
  this->update();
}

void ribi::cmap::QtNode::SetNode(
  const Concept& concept,
  const bool is_center_node,
  const double center_x,
  const double center_y
) noexcept
{

  m_examples = concept.GetExamples();
  m_is_complex = concept.GetIsComplex();
  m_rating_complexity = concept.GetRatingComplexity();
  m_rating_concreteness = concept.GetRatingConcreteness();
  m_rating_specificity = concept.GetRatingSpecificity();

  //::ribi::cmap::SetX(*this, center_x);
  //::ribi::cmap::SetY(*this, center_y);
  const std::string text{::ribi::cmap::GetText(concept)};
  ::ribi::cmap::SetText(*this, text);
  //this->SetText(Wordwrap(node.GetConcept().GetName(), GetWordWrapLength()));

  //this->SetCenterPos(m_node.GetX(), m_node.GetY());
  this->SetCenterPos(center_x, center_y);

  if (is_center_node)
  {
    this->setFlags(
        QGraphicsItem::ItemIsFocusable
      | QGraphicsItem::ItemIsSelectable
    );
    assert(!(flags() & QGraphicsItem::ItemIsMovable));
    assert(IsQtCenterNode(*this));
  }
  else
  {
    this->setFlags(
        QGraphicsItem::ItemIsFocusable
      | QGraphicsItem::ItemIsMovable
      | QGraphicsItem::ItemIsSelectable
    );
    assert(!IsQtCenterNode(*this));
  }

  //Ensures(::ribi::cmap::GetX(*this) == center_x);
  //Ensures(::ribi::cmap::GetY(*this) == center_y);
  Ensures(::ribi::cmap::GetText(*this) == ::ribi::cmap::GetText(concept));
}

void ribi::cmap::SetConcept(QtNode& qtnode, const Concept& concept)
{
  qtnode.SetNode(
    concept,
    IsCenterNode(qtnode),
    GetX(qtnode),
    GetY(qtnode)
  );
}

void ribi::cmap::SetText(QtNode& qtnode, const std::string& text)
{
  qtnode.SetText( { text } );
}

void ribi::cmap::SetX(QtNode& qtnode, const double x)
{
  qtnode.SetCenterX(x);
}

void ribi::cmap::SetY(QtNode& qtnode, const double y)
{
  qtnode.SetCenterY(y);
}

std::string ribi::cmap::QtNode::ToStr() const noexcept
{
  std::stringstream s;
  s << (*this);
  return s.str();
}

std::ostream& ribi::cmap::operator<<(std::ostream& os, const QtNode& qtnode) noexcept
{
  os
    << qtnode.GetExamples() << ','
    << qtnode.GetIsComplex() << ','
    << qtnode.GetRatingComplexity() << ','
    << qtnode.GetRatingConcreteness() << ','
    << qtnode.GetRatingSpecificity()
  ;
  return os;
}
