#ifndef QTCONCEPTMAPNODEITEM_H
#define QTCONCEPTMAPNODEITEM_H

#include <functional>

#include "conceptmapfwd.h"
#include "conceptmapnode.h"
#include "conceptmapnodetype.h"
#include "qtroundededitrectitem.h"

namespace ribi {
namespace cmap {

class QtConceptMapTest;
class QtConceptMapQtNodeTest;

///QtNode displays a Node as a QtConceptMapElement
///Instead of duplicating state, Node's members
///are scattered over the class
struct QtNode final : public QtRoundedEditRectItem
{
  ~QtNode() noexcept;

  explicit QtNode(
    const Node& node = Node(),
    QGraphicsItem* parent = 0
  );
  explicit QtNode(
    const Concept& concept,
    const int id,
    const NodeType type,
    const double center_x = 0.0,
    const double center_y = 0.0,
    QGraphicsItem* parent = 0
  );

  const auto& GetExamples() const noexcept { return m_examples; }
  int GetId() const noexcept { return m_id; }

  ///The concept on an edge (only) being complex,
  ///as rated by an assessor.
  ///This is different than an example being complex.
  auto GetIsComplex() const noexcept { return m_is_complex; }
  int GetRatingComplexity() const noexcept { return m_rating_complexity; }
  int GetRatingConcreteness() const noexcept { return m_rating_concreteness; }
  int GetRatingSpecificity() const noexcept { return m_rating_specificity; }

  auto GetNodeType() const noexcept { return m_node_type; }

  ///Sets the function that determines the brush of the QtNode
  void SetBrushFunction(const std::function<QBrush(const ribi::cmap::QtNode&)>& f) noexcept;

  void SetExamples(const Examples examples) noexcept;

  ///m_is_complex is used to determine if a concept is complex.
  ///In this context, m_is_complex is used to determine whether a QtNode and
  ///whether a QtEdge is complex
  void SetIsComplex(const bool is_complex = true) noexcept { m_is_complex = is_complex; }

  void SetNode(
    const Concept& concept,
    const NodeType type,
    const double center_x = 0.0,
    const double center_y = 0.0
  ) noexcept;

  ///Set the rating of this Concept for complexity
  ///-1: not rated, 0: lowest, 2: highest
  void SetRatingComplexity(const int rating_complexity);

  ///Set the rating of this Concept for concreteness
  ///-1: not rated, 0: lowest, 2: highest
  void SetRatingConcreteness(const int rating_concreteness);

  ///Set the rating of this Concept for specificity
  ///-1: not rated, 0: lowest, 2: highest
  void SetRatingSpecificity(const int rating_specificity);

  ///Sets the function that determines the brush of the QtNode
  void SetVignetteBrushFunction(const std::function<QBrush(const ribi::cmap::QtNode&)>& f) noexcept;

  std::string ToStr() const noexcept;

  virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem *, QWidget *) noexcept final;

  ///Define a usertype for this QGraphicsItem, must be unique
  enum { Type = UserType + 3 };
  int type() const override
  {
    return Type;
  }

protected:
  void focusInEvent(QFocusEvent *event) noexcept final override;
  void focusOutEvent(QFocusEvent *event) noexcept final override;
  void hoverMoveEvent(QGraphicsSceneHoverEvent *event) noexcept final;
  void keyPressEvent(QKeyEvent *event) noexcept final;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override final;

private:

  ///The function that determines this QtNode its brush
  std::function<QBrush(const ribi::cmap::QtNode&)> m_brush_function;

  ///The node being edited, or displayed and not changed, or rated
  Examples m_examples;

  ///ID of the Node it must represent. Like for the Node,
  ///it must be unique. Used in constructing
  ///the concept map
  const int m_id;

  ///m_is_complex is used to determine if a concept is complex.
  ///In this context, m_is_complex is used to determine whether a QtNode and
  ///whether a QtEdge is complex
  bool m_is_complex = false;
  int m_rating_complexity = -1;
  int m_rating_concreteness = -1;
  int m_rating_specificity = -1;

  bool m_show_bounding_rect;

  NodeType m_node_type;

  ///The function that determines this QtNode's vignette's brush
  std::function<QBrush(const ribi::cmap::QtNode&)> m_vignette_brush_function;

  friend class QtConceptMapTest;
  friend class QtConceptMapQtNodeTest;
};

///Count the number of examples
int CountExamples(const QtNode& qtnode) noexcept;

///Create the flags for the node, when in Edit mode
QGraphicsItem::GraphicsItemFlags CreateEditFlags(const QtNode& qtnode) noexcept;

///Create the flags for the node, when in Rate mode
QGraphicsItem::GraphicsItemFlags CreateRateFlags(const QtNode& qtnode) noexcept;

///Create the flags for the node, when in Uninitialized mode
QGraphicsItem::GraphicsItemFlags CreateUninitializedFlags(const QtNode& qtnode) noexcept;

///Get the center of the QtNode
QPointF GetCenterPos(const QtNode& qtnode) noexcept;

///Get the Concept of the QtNode its Node
Concept GetConcept(const QtNode& qtnode) noexcept;

///Get the Example of the QtNode its Node its Concept
const Examples& GetExamples(const QtNode& qtnode) noexcept;

///Get the Z order of each node,
///which should be QtEdgeArrow < QtEdge < 0.0 < QtNode < QtNewArrow < QtTool
constexpr double GetQtNodeZvalue() { return 0.5; }

int GetRatingComplexity(const QtNode& qtnode) noexcept;
int GetRatingConcreteness(const QtNode& qtnode) noexcept;
int GetRatingSpecificity(const QtNode& qtnode) noexcept;

int GetId(const QtNode& qtnode) noexcept;
std::string GetName(const QtNode& qtnode) noexcept;
Node GetNode(const QtNode& qtnode) noexcept;
std::string GetText(const QtNode& qtnode) noexcept;
NodeType GetType(const QtNode& qtnode) noexcept;

///Number of characters for wordwrapping
constexpr int GetWordWrapLength() { return 40; }

///Get the x coordinat of the center of the QtNode
double GetX(const QtNode& qtnode) noexcept;

///Get the x coordinat of the center of the QtNode
double GetY(const QtNode& qtnode) noexcept;

///Does the QtNode its Node have at least one example?
bool HasExamples(const QtNode& qtnode) noexcept;

bool IsCenterNode(const QtNode& qtnode) noexcept;
bool IsComplex(const QtNode& qtnode) noexcept;


///Is this QtNode in the center on a QtEdge?
///Simple: if it has a parent item (a QtEdge),
///it is on a QtEdge
bool IsOnEdge(const QtNode& qtnode) noexcept;

bool IsMovable(const QtNode& qtnode) noexcept;

///Is this QGraphicsItem an QtNode on an edge, instead of an autonomous QtNode?
bool IsQtNodeOnEdge(
  const QGraphicsItem * const item
) noexcept;

bool IsSelectable(const QtNode& qtnode) noexcept;
bool IsSelected(const QtNode& qtnode) noexcept;

///Move a QtNode (and its Node) relatively
void Move(QtNode& qtnode, const double dx, const double dy);

std::function<bool(const QtNode* const)> QtNodeHasId(const int id);
std::function<bool(const QtNode* const)> QtNodeHasName(const std::string& name);

void SetConcept(QtNode& qtnode, const Concept& concept);

void SetExamples(QtNode& qtnode, const Examples& examples) noexcept;

///Set the tallied complexity
void SetIsComplex(QtNode& qtnode, const bool is_complex = true);

///Set the rating of this Node for complexity
///-1: not rated, 0: lowest, 2: highest
void SetRatingComplexity(QtNode& qtnode, const int rating_complexity);

///Set the rating of this Node for concreteness
///-1: not rated, 0: lowest, 2: highest
void SetRatingConcreteness(QtNode& qtnode, const int rating_concreteness);

///Set the rating of this Node for specificity
///-1: not rated, 0: lowest, 2: highest
void SetRatingSpecificity(QtNode& qtnode, const int rating_specificity);

void SetText(QtNode& qtnode, const std::string& text);
void SetX(QtNode& qtnode, const double x);
void SetY(QtNode& qtnode, const double y);


std::ostream& operator<<(std::ostream& os, const QtNode& qtnode) noexcept;

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPNODEITEM_H
