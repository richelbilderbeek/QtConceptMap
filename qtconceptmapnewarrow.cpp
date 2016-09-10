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
#include "qtconceptmapnewarrow.h"

#include <string>
#include <boost/lexical_cast.hpp>

#include <QFont>
#include <QPainter>
#include <QDebug>

#include "geometry.h"
#include "qtconceptmapqtnode.h"
#include "trace.h"

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#pragma GCC diagnostic pop

ribi::cmap::QtNewArrow::QtNewArrow()
  : QtArrowItem(0.0, 0.0, false, 0.0, 0.0, true),
    m_from{nullptr}
{
  //New arrows are above all items
  this->setZValue(1.0);

  //A new arrow must not be moveable
  this->setFlag(QGraphicsItem::ItemIsSelectable,false);
  this->setFlag(QGraphicsItem::ItemIsMovable,false);
  this->setFlag(QGraphicsItem::ItemIsFocusable,false);

  //Reject enterHoverEvents
  this->setAcceptHoverEvents(false);

  assert(!(this->flags() & QGraphicsItem::ItemIsSelectable));
  assert(!(this->flags() & QGraphicsItem::ItemIsMovable));
  assert(!(this->flags() & QGraphicsItem::ItemIsFocusable));
  assert(!this->isSelected());
}

void ribi::cmap::QtNewArrow::Start(QtNode * const from)
{
  assert(from);
  m_from = from;
  this->SetTailPos(from->GetCenterX(), from->GetCenterY());
  this->SetHasTail(false);
  this->SetHeadPos(from->GetCenterX() + 16, from->GetCenterY() - 16);
  this->SetHasHead(true);
  this->show();
  assert(!this->isSelected());
}

void ribi::cmap::QtNewArrow::paint(
  QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget
)
{
  assert(!this->isSelected());
  assert(!this->hasFocus());
  assert(m_from);
  typedef boost::geometry::model::d2::point_xy<double> Point;
  typedef boost::geometry::model::linestring<Point> Line;
  typedef boost::geometry::model::box<Point> Rect;
  const Line line = Geometry().CreateLine(
    std::vector<Point>(
      {
        Point(m_from->GetCenterX(),m_from->GetCenterY()),
        Point(this->line().p2().x(),this->line().p2().y())
      }
    )
  );
  const QRectF qr1 = m_from->boundingRect().translated(m_from->GetCenterPos());

  const Rect r1(
    Point(qr1.topLeft().x()    ,qr1.topLeft().y()    ),
    Point(qr1.bottomRight().x(),qr1.bottomRight().y())
    );

  std::vector<Point> p1 = Geometry().GetLineRectIntersections(line,r1);
  if (p1.empty())
  {
    //Yes,it happens, when the line does not leave the rectangle
    //this happens when the two node rectanges overlap
    p1.push_back(Point(m_from->GetCenterX(),m_from->GetCenterY()));
  }
  assert(!p1.empty());
  assert(p1.size() == 1);
  const QPointF p2 = this->line().p2();
  this->setLine(QLineF(QPointF(p1[0].x(),p1[0].y()),p2));
  QtArrowItem::paint(painter,option,widget);
  assert(!this->isSelected());
  assert(!this->hasFocus());
}
