#ifndef QTCONCEPTMAPNEWARROW_H
#define QTCONCEPTMAPNEWARROW_H

#include <string>
#include <vector>






#include <QBrush>
#include "qtarrowitem.h"
#include "qtconceptmapfwd.h"


namespace ribi {
namespace cmap {

///QtNewArrow is the arrow shown before a new one is added
struct QtNewArrow : public QtArrowItem
{
  QtNewArrow();

  ///Set and show the arrow in itis initial condition
  void Start(QtNode * const from);

  QtNewArrow(const QtNewArrow&) = delete;
  QtNewArrow& operator=(const QtNewArrow&) = delete;

  ///Obtain the source node, can be nullptr
  const QtNode * GetFrom() const noexcept;
  QtNode * GetFrom() noexcept;

  private:
  ///Must be suppplied
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

  ///The source node
  ///Cannot be const as the user might want to edit it
  QtNode * m_from;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPNEWARROW_H
