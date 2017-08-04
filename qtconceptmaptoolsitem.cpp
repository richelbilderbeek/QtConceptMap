#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "qtconceptmaptoolsitem.h"

#include <cmath>
#include <cassert>
#include <QCursor>
#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include "qtconceptmapqtnode.h"

#pragma GCC diagnostic pop

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

  this->setZValue(2.0);
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
  assert(m_item);
  assert(this->scene());
  this->setPos(
    m_item->GetCenterX(),
    m_item->GetCenterY() - (m_item->GetOuterHeight() / 2.0) - 16.0
  );

  //Must be close
  assert(std::abs(this->x() - m_item->GetCenterX()) < 1.0);
  assert(
    std::abs(
      m_item->GetCenterY() - (m_item->GetOuterHeight() / 2.0) - 16.0 - this->y()
    ) < 1.0
  );

  QGraphicsPixmapItem::paint(painter,option,widget);
}

void ribi::cmap::QtTool::SetBuddyItem(QtNode * const item)
{
  m_item = item;
  this->setVisible(m_item);
  this->update();
}

void ribi::cmap::QtTool::setVisible(bool visible)
{
  QGraphicsPixmapItem::setVisible(visible);
}

