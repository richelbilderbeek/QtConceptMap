#ifndef CONCEPTMAPCOMMANDMOVE_H
#define CONCEPTMAPCOMMANDMOVE_H

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

///Start a new node
///-Can be used only when there is an existing concept map
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
  ~CommandMove() noexcept {}

  auto GetDx() const noexcept { return m_dx; }
  auto GetDy() const noexcept { return m_dy; }

  ///Get the Node or Edge its name
  std::string GetName() const noexcept { return m_name; }
  void redo() override;
  void undo() override;

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

/// Works on, for example  'create_new_node(0, 0, from)'
CommandMove * parse_command_move(
  QtConceptMap& qtconceptmap, std::string s);

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCOMMANDMOVE_H
