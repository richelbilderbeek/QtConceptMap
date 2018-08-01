#include "qtconceptmapqtnode.h"

#include <cassert>
#include <climits>

#include <QCursor>
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
#include "qtconceptmapqtedge.h"
#include "qtconceptmapqtnodefactory.h"

ribi::cmap::QtNode::QtNode(
  const Node& node,
  QGraphicsItem* parent
) : QtNode(
      node.GetConcept(),
      node.GetId(),
      node.GetType(),
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
  const NodeType type,
  const double center_x,
  const double center_y,
  QGraphicsItem* parent)
  : QtRoundedEditRectItem(
      Wordwrap(concept.GetName(), GetWordWrapLength()),
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
    m_show_bounding_rect{false},
    m_vignette_brush_function{GetQtNodeVignetteBrushFunctionUninitialized()}
{
  //Allow mouse tracking
  this->setAcceptHoverEvents(true);

  this->SetPadding(QtRoundedEditRectItem::Padding(1.0, 6.0, 1.0, 2.0));

  this->setAcceptHoverEvents(true);
  this->setZValue(GetQtNodeZvalue());
  this->SetContourPen(QPen(Qt::black, 1.0));
  this->SetFocusPen(QPen(Qt::black, 1.0, Qt::DashLine));
  SetNode(concept, type, center_x, center_y);
  assert(m_id == id);
  assert(GetType(*this) == type);
  assert(IsMovable(*this) != IsCenterNode(*this));
}

ribi::cmap::QtNode::~QtNode() noexcept
{

}

QGraphicsItem::GraphicsItemFlags ribi::cmap::CreateEditFlags(
  const QtNode& qtnode) noexcept
{
  if (qtnode.GetNodeType() == NodeType::center)
  {
    return
        QGraphicsItem::ItemIsFocusable
      | QGraphicsItem::ItemIsSelectable
    ;
  }
  assert(qtnode.GetNodeType() != NodeType::center);
  return
      QGraphicsItem::ItemIsFocusable
    | QGraphicsItem::ItemIsMovable
    | QGraphicsItem::ItemIsSelectable
  ;
}

QGraphicsItem::GraphicsItemFlags ribi::cmap::CreateRateFlags(
  const QtNode&) noexcept
{
  return
      QGraphicsItem::ItemIsFocusable
    | QGraphicsItem::ItemIsSelectable
  ;
}

QGraphicsItem::GraphicsItemFlags ribi::cmap::CreateUninitializedFlags(
  const QtNode&) noexcept
{
  return 0;
}

void ribi::cmap::QtNode::dragEnterEvent(QGraphicsSceneDragDropEvent * event)
{
  assert(!"ribi::cmap::QtNode::dragEnterEvent is never called");
  QtRoundedEditRectItem::dragEnterEvent(event);
}

void ribi::cmap::QtNode::dragLeaveEvent(QGraphicsSceneDragDropEvent * event)
{
  assert(!"ribi::cmap::QtNode::dragLeaveEvent is never called");
  QtRoundedEditRectItem::dragLeaveEvent(event);
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
  return qtnode.pos();
}

ribi::cmap::Concept ribi::cmap::GetConcept(const QtNode& qtnode) noexcept
{
  return Concept(
    Unwordwrap(qtnode.GetText()),
    qtnode.GetExamples(),
    qtnode.GetIsComplex(),
    qtnode.GetRatingComplexity(),
    qtnode.GetRatingConcreteness(),
    qtnode.GetRatingSpecificity()
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
  //ID is important, as it is what makes
  // a QtNode/Node uniquely identifyable
  return Node(
    GetConcept(qtnode),
    GetType(qtnode),
    GetX(qtnode),
    GetY(qtnode),
    GetId(qtnode)
  );
}

std::string ribi::cmap::GetText(const QtNode& qtnode) noexcept
{
  return Unwordwrap(qtnode.GetText());
}

ribi::cmap::NodeType ribi::cmap::GetType(const QtNode& qtnode) noexcept
{
  return qtnode.GetNodeType();
}


double ribi::cmap::GetX(const QtNode& qtnode) noexcept
{
  return qtnode.pos().x();
}

double ribi::cmap::GetY(const QtNode& qtnode) noexcept
{
  return qtnode.pos().y();
}

bool ribi::cmap::HasExamples(const QtNode& qtnode) noexcept
{
  return !IsEmpty(qtnode.GetExamples());
}

void ribi::cmap::QtNode::hoverMoveEvent(QGraphicsSceneHoverEvent*) noexcept
{
  this->setCursor(QCursor(Qt::PointingHandCursor));
  std::stringstream s;
  s
    << "QGraphicsItem::ItemIsFocusable: " << static_cast<bool>(flags() & QGraphicsItem::ItemIsFocusable)
    << "\nQGraphicsItem::ItemIsMovable: " << static_cast<bool>(flags() & QGraphicsItem::ItemIsMovable)
    << "\nQGraphicsItem::ItemIsSelectable: " << static_cast<bool>(flags() & QGraphicsItem::ItemIsSelectable)
  ;
  this->setToolTip(s.str().c_str());
}

bool ribi::cmap::IsCenterNode(const QtNode& qtnode) noexcept
{
  return qtnode.GetNodeType() == NodeType::center;
}

bool ribi::cmap::IsComplex(const QtNode& qtnode) noexcept
{
  return qtnode.GetIsComplex();
}

bool ribi::cmap::IsEnabled(const QtNode& qtnode) noexcept
{
  return qtnode.isEnabled();
}

bool ribi::cmap::IsOnEdge(
  const QtNode& qtnode
) noexcept
{
  const auto parent_item = qtnode.parentItem();
  assert(!parent_item || qgraphicsitem_cast<QtEdge*>(parent_item));
  return parent_item;
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
  QtRoundedEditRectItem::keyPressEvent(event);
}

void ribi::cmap::QtNode::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
  QtRoundedEditRectItem::mouseMoveEvent(event);
}

void ribi::cmap::Move(QtNode& qtnode, const double dx, const double dy)
{
  qtnode.moveBy(dx, dy);
}

void ribi::cmap::QtNode::paint(
  QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget
) noexcept
{
  assert(this->scene());
  assert(painter);

  //Set the brush of this QtNode
  this->setBrush(m_brush_function(*this));

  QtRoundedEditRectItem::paint(painter,item,widget);

  if (HasExamples(*this))
  {
    painter->setBrush(m_vignette_brush_function(*this));
    //painter->setBrush(Qt::transparent);
    painter->setPen(Qt::black);
    painter->drawRect(
      (GetInnerRect().width() / 2.0) - 5.0,
      -(GetInnerRect().height() / 2.0) + 3.0,
      3.0,
      3.0
    );
  }

  painter->setBrush(Qt::transparent);

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

void ribi::cmap::QtNode::SetExamples(const Examples examples) noexcept
{
  m_examples = examples;
}

void ribi::cmap::QtNode::SetNode(
  const Concept& concept,
  const NodeType type,
  const double center_x,
  const double center_y
) noexcept
{

  m_examples = concept.GetExamples();
  m_is_complex = concept.GetIsComplex();
  m_rating_complexity = concept.GetRatingComplexity();
  m_rating_concreteness = concept.GetRatingConcreteness();
  m_rating_specificity = concept.GetRatingSpecificity();
  m_node_type = type;

  const std::string text{::ribi::cmap::GetText(concept)};
  ::ribi::cmap::SetText(*this, text);
  this->SetCenterPos(center_x, center_y);
  this->setFlags(CreateEditFlags(*this));
  Ensures(::ribi::cmap::GetText(*this) == ::ribi::cmap::GetText(concept));
}

void ribi::cmap::SetConcept(QtNode& qtnode, const Concept& concept)
{
  qtnode.SetNode(
    concept,
    GetType(qtnode),
    GetX(qtnode),
    GetY(qtnode)
  );
}

void ribi::cmap::SetExamples(QtNode& qtnode, const Examples& examples) noexcept
{
  qtnode.SetExamples(examples);
}

void ribi::cmap::QtNode::SetRatingComplexity(const int rating_complexity)
{
  if (rating_complexity < -1 || rating_complexity > 2)
  {
    throw std::invalid_argument(
      "ribi::cmap::QtNode::SetRatingComplexity: invalid complexity"
    );
  }
  m_rating_complexity = rating_complexity;
  Ensures(m_rating_complexity >= -1); //!OCLINT not a double negative in practice
  Ensures(m_rating_complexity <=  2); //!OCLINT not a double negative in practice
}

void ribi::cmap::SetRatingComplexity(
  QtNode& qtnode,
  const int rating_complexity
)
{
  qtnode.SetRatingComplexity(rating_complexity);
}

void ribi::cmap::QtNode::SetRatingConcreteness(const int rating_concreteness)
{
  if (rating_concreteness < -1 || rating_concreteness > 2)
  {
    throw std::invalid_argument(
      "ribi::cmap::QtNode::SetRatingConcreteness: invalid concreteness"
    );
  }
  m_rating_concreteness = rating_concreteness;
  Ensures(m_rating_concreteness >= -1); //!OCLINT not a double negative in practice
  Ensures(m_rating_concreteness <=  2); //!OCLINT not a double negative in practice
}

void ribi::cmap::SetRatingConcreteness(
  QtNode& qtnode,
  const int rating_concreteness
)
{
  qtnode.SetRatingConcreteness(rating_concreteness);
}

void ribi::cmap::QtNode::SetRatingSpecificity(const int rating_specificity)
{
  if (rating_specificity < -1 || rating_specificity > 2)
  {
    throw std::invalid_argument(
      "ribi::cmap::QtNode::SetRatingSpecificity: invalid specificity"
    );
  }
  m_rating_specificity = rating_specificity;
  Ensures(m_rating_specificity >= -1); //!OCLINT not a double negative in practice
  Ensures(m_rating_specificity <=  2); //!OCLINT not a double negative in practice
}

void ribi::cmap::SetRatingSpecificity(
  QtNode& qtnode,
  const int rating_specificity
)
{
  qtnode.SetRatingSpecificity(rating_specificity);
}

void ribi::cmap::SetText(QtNode& qtnode, const std::string& text)
{
  qtnode.SetText( Wordwrap(text, GetWordWrapLength()) );
}

void ribi::cmap::QtNode::SetVignetteBrushFunction(
  const std::function<QBrush(const ribi::cmap::QtNode&)>& f
) noexcept
{
  m_vignette_brush_function = f;
  this->update();
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
    << qtnode.GetNodeType() << ','
    << qtnode.GetRatingComplexity() << ','
    << qtnode.GetRatingConcreteness() << ','
    << qtnode.GetRatingSpecificity()
  ;
  return os;
}
