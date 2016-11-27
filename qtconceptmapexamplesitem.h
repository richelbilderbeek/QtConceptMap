#ifndef QTCONCEPTMAPEXAMPLESITEM_H
#define QTCONCEPTMAPEXAMPLESITEM_H

#include "qtroundededitrectitem.h"
#include "qtconceptmapfwd.h"

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

  ///Set the position of the QtExamplesItem
  void Reposition();
};

///Is the QtExamplesItem close to the QtNode?
bool IsClose(const QtExamplesItem& item, const QtNode& qtnode) noexcept;

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPEXAMPLESITEM_H
