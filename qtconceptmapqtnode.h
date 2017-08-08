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

class qtconceptmap_test;
class qtconceptmapqtnode_test;

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

  friend class qtconceptmap_test;
  friend class qtconceptmapqtnode_test;
};

void CheckInvariants(const QtNode& qtnode) noexcept;

std::string GetText(const QtNode& qtnode) noexcept;

///Number of characters for wordwrapping
constexpr int GetWordWrapLength() { return 80; }

std::string GetText(const QtNode& qtnode) noexcept;

///Get the x coordinat of the center of the QtNode
double GetX(const QtNode& qtnode) noexcept;

///Get the x coordinat of the center of the QtNode
double GetY(const QtNode& qtnode) noexcept;

bool HasExamples(const QtNode& qtnode) noexcept;

bool IsCenterNode(const QtNode& qtnode) noexcept;

///Move a QtNode (and its Node) relatively
void Move(QtNode& qtnode, const double dx, const double dy);

void SetText(QtNode& qtnode, const std::string& text);
void SetX(QtNode& qtnode, const double x);
void SetY(QtNode& qtnode, const double y);


std::ostream& operator<<(std::ostream& os, const QtNode& qtnode) noexcept;

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPNODEITEM_H
