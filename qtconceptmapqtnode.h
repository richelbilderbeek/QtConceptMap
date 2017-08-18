#ifndef QTCONCEPTMAPNODEITEM_H
#define QTCONCEPTMAPNODEITEM_H

#include <functional>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "qtroundededitrectitem.h"
#include "conceptmapnode.h"
#include "conceptmapfwd.h"
#pragma GCC diagnostic pop

namespace ribi {
namespace cmap {

class QtConceptMapTest;
class QtConceptMapQtNodeTest;

///QtNode displays a Node as a QtConceptMapElement
struct QtNode : public QtRoundedEditRectItem
{
  virtual ~QtNode() noexcept;

  ///Node cannot be const as it contains a Concept that the user might want to edit
  ///concept_item contains the Stategy to display (and respond to the concept)
  explicit QtNode(const Node& node, QGraphicsItem* parent = 0);

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

  //void focusInEvent(QFocusEvent *event) noexcept final override;
  //void focusOutEvent(QFocusEvent *event) noexcept final override;
  void keyPressEvent(QKeyEvent *event) noexcept final;
  void hoverMoveEvent(QGraphicsSceneHoverEvent *event) noexcept final;

private:

  ///The function that determines this QtNode its brush
  std::function<QBrush(const ribi::cmap::QtNode&)> m_brush_function;

  ///The node being edited, or displayed and not changed, or rated
  Node m_node;

  bool m_show_bounding_rect;

  friend class QtConceptMapTest;
  friend class QtConceptMapQtNodeTest;
};

void CheckInvariants(const QtNode& qtnode) noexcept;

///Get the center of the QtNode
QPointF GetCenterPos(const QtNode& qtnode) noexcept;

///Get the Concept of the QtNode its Node
const Concept& GetConcept(const QtNode& qtnode) noexcept;

///Get the Example of the QtNode its Node its Concept
const Examples& GetExamples(const QtNode& qtnode) noexcept;

std::string GetText(const QtNode& qtnode) noexcept;

///Number of characters for wordwrapping
constexpr int GetWordWrapLength() { return 80; }

///Get the x coordinat of the center of the QtNode
double GetX(const QtNode& qtnode) noexcept;

///Get the x coordinat of the center of the QtNode
double GetY(const QtNode& qtnode) noexcept;

///Does the QtNode its Node have at least one example?
bool HasExamples(const QtNode& qtnode) noexcept;

bool IsCenterNode(const QtNode& qtnode) noexcept;
bool IsEnabled(const QtNode& qtnode) noexcept;
bool IsMovable(const QtNode& qtnode) noexcept;
bool IsSelectable(const QtNode& qtnode) noexcept;
bool IsSelected(const QtNode& qtnode) noexcept;
bool IsVisible(const QtNode& qtnode) noexcept;

///Move a QtNode (and its Node) relatively
void Move(QtNode& qtnode, const double dx, const double dy);

///Functor
std::function<bool(const QtNode* const)> QtNodeHasName(const std::string& name);

void SetConcept(QtNode& qtnode, const Concept& concept);
void SetText(QtNode& qtnode, const std::string& text);
void SetX(QtNode& qtnode, const double x);
void SetY(QtNode& qtnode, const double y);


std::ostream& operator<<(std::ostream& os, const QtNode& qtnode) noexcept;

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPNODEITEM_H
