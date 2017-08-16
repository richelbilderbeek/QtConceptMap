#ifndef CONCEPTMAPCOMMANDMOVENODE_H
#define CONCEPTMAPCOMMANDMOVENODE_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"

#include "conceptmapnode.h"
#include "conceptmap.h"
#include "qtconceptmapqtnode.h"
#include <QGraphicsScene>
#include "qtconceptmaptoolsitem.h"
#include "qtconceptmapcommand.h"
#include "qtconceptmapmode.h"
#pragma GCC diagnostic pop

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
    const std::function<bool(const QtNode&)>& predicate,
    const double dx,
    const double dy
  );
  CommandMoveNode(const CommandMoveNode&) = delete;
  CommandMoveNode& operator=(const CommandMoveNode&) = delete;
  ~CommandMoveNode() noexcept {}

  auto GetDx() const noexcept { return m_dx; }
  auto GetDy() const noexcept { return m_dy; }

  /// Get the (first) QtNode in the QtConceptMap that satisfied the predicate
  const QtNode * GetMovedQtNode() const noexcept { return m_moved_qtnode; }

  auto GetPredicate() const noexcept { return m_predicate; }

  ///Will throw if nothing is moved
  void redo() override;

  void undo() override;

  private:

  ///The first QtNode (not on Edge) in the QtConceptMap satisfying the
  ///predicate
  QtNode * m_moved_qtnode;

  ///@param predicate function to find the first QtNode (not on Edge)
  ///  in the QtConceptMap
  const std::function<bool(const QtNode&)> m_predicate;

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
