#include "qtconceptmaptoolsitem.h"

#include <cassert>
#include <cmath>

#include <QCursor>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

#include "qtconceptmap.h"
#include "qtconceptmapqtnode.h"

ribi::cmap::QtTool::QtTool()
  : m_qtnode(nullptr)
{
  this->setPixmap(QPixmap(":/QtConceptMap/pics/ToolIcon.png"));

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

ribi::cmap::QtTool::~QtTool()
{

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
  if (!m_qtnode) return;

  assert(this->isVisible());

  this->setPos(
    m_qtnode->pos().x(),
    m_qtnode->pos().y() - (m_qtnode->GetOuterHeight() / 2.0) - 16.0
  );

  //Must be close
  assert(std::abs(this->x() - m_qtnode->pos().x()) < 1.0);
  assert(
    std::abs(
      m_qtnode->pos().y() - (m_qtnode->GetOuterHeight() / 2.0) - 16.0 - this->y()
    ) < 1.0
  );
}

void ribi::cmap::QtTool::SetBuddyItem(QtNode * const qtnode)
{
  m_qtnode = qtnode;
  this->setVisible(m_qtnode != nullptr);
  Reposition();
}

void ribi::cmap::QtTool::setVisible(bool visible)
{
  QGraphicsPixmapItem::setVisible(visible);
}

