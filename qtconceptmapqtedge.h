#ifndef QTCONCEPTMAPEDGEITEM_H
#define QTCONCEPTMAPEDGEITEM_H

#include <QGraphicsItem>
#include "qtconceptmapfwd.h"
#include "qtconceptmapqtnode.h"
#include "conceptmapedge.h"

namespace ribi {

struct QtRoundedRectItem;

namespace cmap {

struct Edge;
struct Node;

///The QtEdge is a QGraphicsItem that
///draws a curve underneath itself, between head and tail arrowhead
///It parents an arrow and a QtNode.
struct QtEdge : public QGraphicsItem
{
  QtEdge(
    const Concept& concept,
    const double x,
    const double y,
    QtNode * const from,
    QtNode * const to
  );
  QtEdge(const QtEdge&) = delete;
  QtEdge& operator=(const QtEdge&) = delete;
  ~QtEdge() noexcept;

  QRectF boundingRect() const override final;

  const QtQuadBezierArrowItem * GetArrow() const noexcept { return m_arrow; }
        QtQuadBezierArrowItem * GetArrow()       noexcept { return m_arrow; }

  ///The node item the arrow originates from
  const QtNode * GetFrom() const noexcept { return m_from; }
        QtNode * GetFrom()       noexcept { return m_from; }

  ///The node item the arrow targets
  const QtNode * GetTo() const noexcept { return m_to; }
        QtNode * GetTo()       noexcept { return m_to; }

  const QtNode * GetQtNode() const noexcept { return m_qtnode; }
        QtNode * GetQtNode()       noexcept { return m_qtnode; }

  bool IsSelected() const;

  void SetConcept(const Concept& concept) noexcept;

  void SetHasHeadArrow(const bool has_head_arrow) noexcept;
  void SetHasTailArrow(const bool has_tail_arrow) noexcept;

  void SetSelected(const bool selected);

  void SetShowBoundingRect(const bool show_bounding_rect) const noexcept
  {
    m_show_bounding_rect = show_bounding_rect;
  }

  //void keyPressEvent(QKeyEvent *event) noexcept override final;
  int type() const override { return UserType + 4; }

protected:
  //void focusInEvent(QFocusEvent *event) noexcept override final;
  //void focusOutEvent(QFocusEvent *event) noexcept override final;
  void mousePressEvent(QGraphicsSceneMouseEvent *event) noexcept override final;
  void paint(
    QPainter* painter,
    const QStyleOptionGraphicsItem* option,
    QWidget* widget
  ) noexcept override final;

  QPainterPath shape() const noexcept override final;

private:
  ///The arrow used for drawing, deleted by this class
  ///Cannot make this const, as I need an initialized m_qtnode
  ///before I can construct this
  QtQuadBezierArrowItem *  m_arrow;

  ///The concept, part of QtNode
  //Concept m_concept;

  ///The node item the arrow originates from
  QtNode * const m_from;

  ///The QtNode around Edge its Node
  QtNode * const m_qtnode;

  ///Show the bounding rectangle
  mutable bool m_show_bounding_rect;

  ///The node item the arrow targets
  QtNode * const m_to;

  ///Will throw if input is invalid
  void CheckInput(QtNode * const from, QtNode * const to);

  ///Use IsSelected instead
  bool isSelected() const = delete;

  ///Use SetSelected instead
  void setSelected(bool selected) = delete;

  ///Shows the bounding rectangle if desired
  void ShowBoundingRect(QPainter* const painter);
};

void CheckInvariants(const QtEdge& qtedge);
void DisableAll(QtEdge& qtedge) noexcept;
void EnableAll(QtEdge& qtedge) noexcept;

QGraphicsItem::GraphicsItemFlags GetQtNodeFlags() noexcept;

Concept GetConcept(const QtEdge& qtedge) noexcept;

std::string GetText(const QtEdge& qtedge) noexcept;

///Get the coordinat of the center of the QtNode at the QtEdge
QPointF GetCenterPos(const QtEdge& qtedge) noexcept;

///Get the x coordinat of the center of the QtNode at the QtEdge
double GetX(const QtEdge& qtedge) noexcept;

///Get the x coordinat of the center of the QtNode at the QtEdge
double GetY(const QtEdge& qtedge) noexcept;

bool HasExamples(const QtEdge& qtnode) noexcept;
bool HasHeadArrow(const QtEdge& qtedge) noexcept;
bool HasTailArrow(const QtEdge& qtedge) noexcept;

///Checks if the QtEdge is connected to a QtNode that is
///a center node
bool IsConnectedToCenterNode(const QtEdge& qtedge);

bool IsEnabled(const QtEdge& qtedge) noexcept;
bool IsMovable(const QtEdge& qtedge) noexcept;
bool IsSelectable(const QtEdge& qtedge) noexcept;
bool IsSelected(const QtEdge& qtedge) noexcept;
bool IsVisible(const QtEdge& qtedge) noexcept;

///Move a QtEdge (and its Edge) relatively
void Move(QtEdge& qtedge, const double dx, const double dy);

///Functor
std::function<bool(QtEdge* const)> QtEdgeHasName(const std::string& name);

void SetConcept(QtEdge& qtedge, const Concept& concept) noexcept;

std::string ToStr(const QtEdge& qtedge) noexcept;

std::ostream& operator<<(std::ostream& os, const QtEdge& qtedge) noexcept;
bool operator==(const QtEdge& lhs, const QtEdge& rhs) noexcept;

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPEDGEITEM_H
