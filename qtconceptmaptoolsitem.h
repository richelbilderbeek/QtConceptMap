
/*














*/



#ifndef QTCONCEPTMAPTOOLSITEM_H
#define QTCONCEPTMAPTOOLSITEM_H

#include <string>
#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <QBrush>
#include <QGraphicsPixmapItem>

#include <boost/shared_ptr.hpp>

#include "conceptmapnode.h"

#include "qtconceptmapfwd.h"
#pragma GCC diagnostic pop

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

  ///Set the position from the widget it floats above
  ///item cannot be const, as the user might want to modify it
  void SetBuddyItem(QtNode * const item);

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

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPTOOLSITEM_H
