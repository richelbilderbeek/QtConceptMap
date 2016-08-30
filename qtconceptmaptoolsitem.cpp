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
#include "qtconceptmaptoolsitem.h"

#include <QCursor>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include "qtconceptmapqtnode.h"

#include "trace.h"
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
  if (item != m_item)
  {
    m_item = item;
    if (m_item)
    {
      this->setVisible(true);
      this->update();
    }
    else
    {
      this->setVisible(false);
    }
  }
}
