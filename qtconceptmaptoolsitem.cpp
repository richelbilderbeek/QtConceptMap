



#include "qtconceptmaptoolsitem.h"

#include <cmath>
#include <cassert>
#include <QCursor>
#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include "qtconceptmapqtnode.h"



ribi::cmap::QtTool::QtTool()
  : m_item(nullptr)
{
  this->setPixmap(QPixmap(":/CppQtConceptMap/images/CppQtConceptMapArrow14x14.png"));

  assert(!this->pixmap().isNull());

  this->setFlags(
      QGraphicsItem::ItemIsFocusable
    | QGraphicsItem::ItemIsSelectable
  );

  //this->setFlags(0);

  //Allow mouse tracking
  this->setAcceptHoverEvents(true);

  this->setVisible(false);

  this->setZValue(GetQtToolZvalue());
}

void ribi::cmap::QtTool::hoverMoveEvent(QGraphicsSceneHoverEvent *)
{
  this->setCursor(QCursor(Qt::PointingHandCursor));
}

void ribi::cmap::QtTool::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
  QGraphicsPixmapItem::mousePressEvent(event);
}

void ribi::cmap::QtTool::paint(
  QPainter* painter,
  const QStyleOptionGraphicsItem* option,
  QWidget* widget
)
{
  QGraphicsPixmapItem::paint(painter,option,widget);
}

void ribi::cmap::QtTool::Reposition() noexcept
{
  if (!m_item) return;

  assert(this->isVisible());

  this->setPos(
    m_item->pos().x(),
    m_item->pos().y() - (m_item->GetOuterHeight() / 2.0) - 16.0
  );

  //Must be close
  assert(std::abs(this->x() - m_item->pos().x()) < 1.0);
  assert(
    std::abs(
      m_item->pos().y() - (m_item->GetOuterHeight() / 2.0) - 16.0 - this->y()
    ) < 1.0
  );
}

void ribi::cmap::QtTool::SetBuddyItem(QtNode * const item)
{
  m_item = item;
  this->setVisible(m_item != nullptr);
  Reposition();
  //this->update();
}

void ribi::cmap::QtTool::setVisible(bool visible)
{
  QGraphicsPixmapItem::setVisible(visible);
}

