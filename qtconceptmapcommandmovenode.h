#ifndef CONCEPTMAPCOMMANDMOVENODE_H
#define CONCEPTMAPCOMMANDMOVENODE_H

#include "qtconceptmapcommand.h"

namespace ribi {
namespace cmap {

///MoveNode a QGraphicsItems that is moveable, in this case QtEdges and non-center QtNodes
class CommandMoveNode final : public Command
{
  public:

  ///@param predicate function to find the first QtNode (not on Edge)
  ///  in the QtConceptMap
  CommandMoveNode(
    QtConceptMap& qtconceptmap,
    QtNode * const qtnode,
    const double dx,
    const double dy
  );
  CommandMoveNode(const CommandMoveNode&) = delete;
  CommandMoveNode& operator=(const CommandMoveNode&) = delete;
  ~CommandMoveNode() noexcept {}

  auto GetDx() const noexcept { return m_dx; }
  auto GetDy() const noexcept { return m_dy; }

  /// Get the (first) QtNode in the QtConceptMap that satisfied the predicate
  const QtNode * GetMovedQtNode() const noexcept { return m_qtnode; }

  ///Will throw if nothing is moved
  void Redo() override;

  void Undo() override;

  private:

  ///The QtNode (not on Edge)
  QtNode * const m_qtnode;

  ///The horizontal relative movement of the node or edge
  const double m_dx;

  ///The vertical relative movement of the node or edge
  const double m_dy;

};

///Predicate to see if a Node has a certain (single-line) text
std::function<bool(const QtNode&)> QtNodeHasText(const std::string& text) noexcept;

/// Works on, for example  'create_new_node(0, 0, from)'
CommandMoveNode * ParseCommandMoveNode(
  QtConceptMap& qtconceptmap, std::string s);


} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCOMMANDMOVENODE_H
