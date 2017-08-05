#include "qtconceptmapqtnode.h"

#include <cassert>
#include <climits>

//#include <boost/lambda/lambda.hpp>

#include <QCursor>
#include <QGraphicsItem>
#include <QKeyEvent>
#include <QPainter>
#include <QPen>

#include "conceptmapconcept.h"
#include "conceptmapexamples.h"
#include "conceptmaphelper.h"
#include "conceptmapnodefactory.h"
#include "conceptmapnode.h"
#include "container.h"
#include "qtconceptmapbrushfactory.h"
#include "qtconceptmaphelper.h"
#include "qtconceptmapqtnodefactory.h"


ribi::cmap::QtNode::QtNode(const Node& node, QGraphicsItem* parent)
  : QtRoundedEditRectItem(
      { "..." },
      ribi::QtRoundedEditRectItem::Padding(),
      QFont("monospace",9),
      parent
    ),
    m_brush_function{GetQtNodeBrushFunctionUninitialized()},
    m_node{node},
    m_show_bounding_rect{false}
{
  //Allow mouse tracking
  this->setAcceptHoverEvents(true);

  this->SetPadding(QtRoundedEditRectItem::Padding(1.0,6.0,1.0,2.0));

  this->setAcceptHoverEvents(true);

  if (IsCenterNode(node))
  {
    this->setFlags(
        QGraphicsItem::ItemIsFocusable
      | QGraphicsItem::ItemIsSelectable
    );
    assert(!(flags() & QGraphicsItem::ItemIsMovable));
  }
  else
  {
    this->setFlags(
        QGraphicsItem::ItemIsFocusable
      | QGraphicsItem::ItemIsMovable
      | QGraphicsItem::ItemIsSelectable
    );
  }


  SetNode(node);
  this->setZValue(0.0);
  this->SetContourPen(QPen(Qt::black, 1.0));
  this->SetFocusPen(QPen(Qt::black, 1.0, Qt::DashLine));
}

ribi::cmap::QtNode::~QtNode() noexcept
{

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

std::string ribi::cmap::GetText(const QtNode& qtnode) noexcept
{
  return GetText(qtnode.GetNode());
}

bool ribi::cmap::HasExamples(const QtNode& qtnode) noexcept
{
  return HasExamples(qtnode.GetNode());
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
  return IsCenterNode(qtnode.GetNode());
}

void ribi::cmap::QtNode::keyPressEvent(QKeyEvent *event) noexcept
{
  QtRoundedEditRectItem::keyPressEvent(event);
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

  if (!GetNode().GetConcept().GetExamples().Get().empty())
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
}

void ribi::cmap::QtNode::SetBrushFunction(
  const std::function<QBrush(const ribi::cmap::QtNode&)>& f
) noexcept
{
  m_brush_function = f;
  this->update();
}

void ribi::cmap::QtNode::SetNode(const Node& node) noexcept
{
  m_node = node;
  this->SetCenterPos(m_node.GetX(), m_node.GetY());
  this->SetText(Wordwrap(node.GetConcept().GetName(), GetWordWrapLength()));
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
    << qtnode.GetNode()
  ;
  return os;
}
