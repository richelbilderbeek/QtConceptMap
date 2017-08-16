#ifndef CONCEPTMAPCOMMANDMOVEEDGE_H
#define CONCEPTMAPCOMMANDMOVEEDGE_H

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

///MoveEdge a QGraphicsItems that is moveable, in this case QtEdges and non-center QtNodes
class CommandMoveEdge final : public Command
{
  public:

  ///@param predicate function to find the find QtEdge that satisfies it
  /// in the QtConceptMap
  CommandMoveEdge(
    QtConceptMap& qtconceptmap,
    const std::function<bool(const QtEdge&)> predicate,
    const double dx,
    const double dy
  );
  CommandMoveEdge(const CommandMoveEdge&) = delete;
  CommandMoveEdge& operator=(const CommandMoveEdge&) = delete;
  ~CommandMoveEdge() noexcept {}

  auto GetDx() const noexcept { return m_dx; }
  auto GetDy() const noexcept { return m_dy; }

  /// If name was an edge name, the renamed edge
  const QtEdge * GetMovedQtEdge() const noexcept { return m_moved_qtedge; }

  /// The predicate
  auto GetPredicate() const noexcept { return m_predicate; }

  void redo() override;
  void undo() override;

  private:

  /// If name was an edge name, the renamed edge
  QtEdge * m_moved_qtedge;

  const std::function<bool(const QtEdge&)> m_predicate;

  ///The horizontal relative movement of the node or edge
  const double m_dx;

  ///The vertical relative movement of the node or edge
  const double m_dy;

};

/// Works on, for example  'move_node(node name, 0, 0)'
CommandMoveEdge * ParseCommandMoveEdge(
  QtConceptMap& qtconceptmap, std::string s);

std::function<bool(const ribi::cmap::QtEdge&)> QtEdgeHasText(
  const std::string& text) noexcept;

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCOMMANDMOVEEDGE_H
