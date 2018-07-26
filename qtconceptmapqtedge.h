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
    const Edge& edge,
    QtNode * const from,
    QtNode * const to
  );
  QtEdge(
    const Node& node,
    QtNode * const from,
    QtNode * const to
  );
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

  ///Construct an Edge from QtEdge members
  Edge GetEdge() const noexcept;

  ///The node item the arrow originates from
  const QtNode * GetFrom() const noexcept { return m_from; }
  ///The node item the arrow originates from
   QtNode * GetFrom() noexcept { return m_from; }

  ///The node item the arrow targets
  const QtNode * GetTo() const noexcept { return m_to; }
  ///The node item the arrow targets
  QtNode * GetTo() noexcept { return m_to; }

  ///The QtNode part of this QtEdge
  const QtNode * GetQtNode() const noexcept { return m_qtnode; }
  ///The QtNode part of this QtEdge
  QtNode * GetQtNode() noexcept { return m_qtnode; }

  ///Is this QtEdge selected?
  bool IsSelected() const;

  ///Set the concept
  void SetConcept(const Concept& concept) noexcept;

  ///Set if this QtEdge has an arrowhead at the head of the arrow
  void SetHasHeadArrow(const bool has_head_arrow) noexcept;

  ///Set if this QtEdge has an arrowhead at the tail of the arrow
  void SetHasTailArrow(const bool has_tail_arrow) noexcept;

  ///Select this QtEdge
  void SetSelected(const bool selected);

  ///Show the bounding rectangle, useful in debugging
  void SetShowBoundingRect(const bool show_bounding_rect) const noexcept
  {
    m_show_bounding_rect = show_bounding_rect;
  }

  ///Define a usertype for this QGraphicsItem, must be unique
  enum { Type = UserType + 4 };
  int type() const override
  {
    return Type;
  }

protected:

  void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override final;
  void mousePressEvent(QGraphicsSceneMouseEvent *event) noexcept override final;


  ///Paint this QGraphicItem
  void paint(
    QPainter* painter,
    const QStyleOptionGraphicsItem* option,
    QWidget* widget
  ) noexcept override final;

  ///Obtain a detailed shape of this QGraphicsItem
  QPainterPath shape() const noexcept override final;

private:
  ///The arrow used for drawing, deleted by this class
  ///Cannot make this const, as I need an initialized m_qtnode
  ///before I can construct this
  QtQuadBezierArrowItem *  m_arrow;

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

Concept GetConcept(const QtEdge& qtedge) noexcept;

std::string GetText(const QtEdge& qtedge) noexcept;

///Get the coordinat of the center of the QtNode at the QtEdge
QPointF GetCenterPos(const QtEdge& qtedge) noexcept;

///Get the Z order of each edge,
///which should be QtEdgeArrow < QtEdge < QtNode < QtNewArrow < QtTool
constexpr double GetQtEdgeZvalue() { return -2.0; }

///Get the Z order of each edge,
///which should be QtEdgeArrow < QtEdge < QtNode < QtNewArrow < QtTool
constexpr double GetQtEdgeArrowZvalue() { return -3.0; }

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
