#ifndef QTCONCEPTMAPTOOLSITEM_H
#define QTCONCEPTMAPTOOLSITEM_H

#include <string>
#include <vector>





#include <QBrush>
#include <QGraphicsPixmapItem>
#include "conceptmapnode.h"
#include "qtconceptmapfwd.h"


namespace ribi {
namespace cmap {

///Tools item is a class that is displayed above a selected node or edge
///and displays clickable icons to perform actions, for example
///adding a relation or modifying arrow edges
///QtTool cannot have its visibility set directly:
///- if it has a buddy, it is visible
///- if it has no buddy, it is invisible
struct QtTool : public QGraphicsPixmapItem
{
  explicit QtTool();
  QtTool(const QtTool& other) = delete;
  QtTool& operator=(const QtTool& other) = delete;
  virtual ~QtTool() noexcept {}

  ///Get the item the tools item floats above
  ///Return type cannot be const, as the user might want to modify it
  QtNode * GetBuddyItem() const { return m_item; }

  ///If the QtTool has a buddy item, reposition it above that item
  ///Does nothing otherwise
  void Reposition() noexcept;

  ///Set the position from the widget it floats above
  ///item cannot be const, as the user might want to modify it
  void SetBuddyItem(QtNode * const item);

  ///Define a type for this item, must be unique
  int type() const override { return UserType + 14; }

  protected:
  void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void paint(QPainter* painter, const QStyleOptionGraphicsItem *, QWidget *);

  private:
  ///Make private to ensure use of other setPos member function
  void setPos(qreal ax, qreal ay) { QGraphicsPixmapItem::setPos(ax,ay); }

  ///The item the tools item floats above
  QtNode * m_item;

  void setVisible(bool visible);
};

///Get the Z order of each tool,
///which should be QtEdgeArrow < QtEdge < QtNode < QtNewArrow < QtTool
constexpr double GetQtToolZvalue() { return 2.0; }

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPTOOLSITEM_H
