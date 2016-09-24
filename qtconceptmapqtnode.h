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
#ifndef QTCONCEPTMAPNODEITEM_H
#define QTCONCEPTMAPNODEITEM_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <boost/shared_ptr.hpp>
#include "qtroundededitrectitem.h"
#include "conceptmapnode.h"
#include "conceptmapfwd.h"
#pragma GCC diagnostic pop

namespace ribi {
namespace cmap {

///QtNode displays a Node as a QtConceptMapElement
struct QtNode : public QtRoundedEditRectItem
{
  virtual ~QtNode() noexcept;

  ///Node cannot be const as it contains a Concept that the user might want to edit
  ///concept_item contains the Stategy to display (and respond to the concept)
  explicit QtNode(const Node& node, QGraphicsItem* parent = 0);

  //virtual QRectF boundingRect() const { return QtRoundedEditRectItem::boundingRect(); }

  void DisableAll();
  void EnableAll();

  const Node& GetNode() const noexcept { return m_node; }
        Node& GetNode()       noexcept { return m_node; }


  ///Sets the function that determines the brush of the QtNode
  void SetBrushFunction(const std::function<QBrush(const ribi::cmap::QtNode&)>& f) noexcept;

  void SetNode(const Node& node) noexcept;

  std::string ToStr() const noexcept;

  virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem *, QWidget *) noexcept final;

  int type() const override { return UserType + 3; }
protected:

  void focusInEvent(QFocusEvent *event) noexcept final override;
  void focusOutEvent(QFocusEvent *event) noexcept final override;
  void keyPressEvent(QKeyEvent *event) noexcept final;
  void hoverMoveEvent(QGraphicsSceneHoverEvent *event) noexcept final;

private:

  ///The function that determines this QtNode its brush
  std::function<QBrush(const ribi::cmap::QtNode&)> m_brush_function;

  ///The node being edited, or displayed and not changed, or rated
  Node m_node;

  bool m_show_bounding_rect;
};

///Number of characters for wordwrapping
constexpr int GetWordWrapLength() { return 80; }

std::ostream& operator<<(std::ostream& os, const QtNode& qtnode) noexcept;

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPNODEITEM_H
