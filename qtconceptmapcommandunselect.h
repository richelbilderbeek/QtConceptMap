#ifndef CONCEPTMAPCOMMANDUNSELECT_H
#define CONCEPTMAPCOMMANDUNSELECT_H

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
class CommandUnselect final : public Command
{
  public:

  ///@param node_or_edge_name the node or edge name/text to be unselected
  CommandUnselect(
    QtConceptMap& qtconceptmap,
    const std::string& node_or_edge_name = ""
  );
  CommandUnselect(const CommandUnselect&) = delete;
  CommandUnselect& operator=(const CommandUnselect&) = delete;
  ~CommandUnselect() noexcept {}

  ///Get the Node or Edge its name
  std::string GetName() const noexcept { return m_name; }
  void redo() override;
  void undo() override;

  private:
  /// If name was an edge name, the renamed edge
  QtEdge * m_renamed_qtedge;

  /// If name was a node name, the renamed node
  QtNode * m_renamed_qtnode;

  ///Text on the QtEdge or QtNode
  const std::string m_name;
};

/// Works on, for example  'create_new_node(0, 0, from)'
CommandUnselect * parse_command_unselect(
  QtConceptMap& qtconceptmap, std::string s);

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCOMMANDUNSELECT_H
