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
#ifndef QTCONCEPTMAPEXAMPLESITEM_H
#define QTCONCEPTMAPEXAMPLESITEM_H

//#include <boost/shared_ptr.hpp>
#include "qtroundededitrectitem.h"
#include "qtconceptmapfwd.h"
//#include "qtconceptmapqtedge.h"

namespace ribi {
namespace cmap {


///QtExamplesItem displays an Examples
struct QtExamplesItem : public QtRoundedEditRectItem
{
  explicit QtExamplesItem(QGraphicsItem* parent = 0);
  QtExamplesItem(const QtExamplesItem&) = delete;
  QtExamplesItem& operator=(const QtExamplesItem&) = delete;
  ~QtExamplesItem() noexcept {}
  ///Check the buddy item
  const QGraphicsItem* GetBuddyItem() const noexcept { return m_item; }

  ///Set the concept this item displays the examples of.
  ///If the concept is nullptr, this item hides
  void SetBuddyItem(const QGraphicsItem* const item);

protected:

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) noexcept;

private:

  ///The concept this item displays the examples of.
  ///If m_concept is nullptr, this item hides
  ///m_item == m_edge if it can be cast, else m_edge == nullptr
  ///m_item == m_node if it can be cast, else m_node == nullptr
  const QGraphicsItem * m_item;
  const QtEdge * m_qtedge; //
  const QtNode * m_qtnode;

  void SetExamples(const Examples& examples);
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPEXAMPLESITEM_H
