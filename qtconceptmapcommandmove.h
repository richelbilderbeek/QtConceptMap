#ifndef CONCEPTMAPCOMMANDMOVE_H
#define CONCEPTMAPCOMMANDMOVE_H

//
//
//
//
#include "qtconceptmapcommand.h"
//

namespace ribi {
namespace cmap {

///Move a QGraphicsItems that is moveable, in this case QtEdges and non-center QtNodes
class CommandMove final : public Command
{
  public:

  ///@param node_or_edge_name the node or edge name/text to be moved
  CommandMove(
    QtConceptMap& qtconceptmap,
    const std::string& node_or_edge_name,
    const double dx,
    const double dy
  );
  CommandMove(const CommandMove&) = delete;
  CommandMove& operator=(const CommandMove&) = delete;
  ~CommandMove() noexcept;

  auto GetDx() const noexcept { return m_dx; }
  auto GetDy() const noexcept { return m_dy; }

  /// If name was an edge name, the renamed edge
  const QtEdge * GetMovedQtEdge() const noexcept { return m_moved_qtedge; }

  /// If name was a node name, the renamed node
  const QtNode * GetMovedQtNode() const noexcept { return m_moved_qtnode; }

  ///Get the Node or Edge its name
  std::string GetName() const noexcept { return m_name; }
  void Redo() override;
  void Undo() override;

  private:
  ///The horizontal relative movement of the node or edge
  const double m_dx;

  ///The vertical relative movement of the node or edge
  const double m_dy;

  /// If name was an edge name, the renamed edge
  QtEdge * m_moved_qtedge;

  /// If name was a node name, the renamed node
  QtNode * m_moved_qtnode;

  ///Text on the QtEdge or QtNode
  const std::string m_name;
};

/// Works on, for example  'move(edge or node name, 0, 0)'
CommandMove * ParseCommandMove(
  QtConceptMap& qtconceptmap, std::string s);


} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCOMMANDMOVE_H
