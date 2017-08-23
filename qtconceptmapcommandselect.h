#ifndef CONCEPTMAPCOMMANDSELECT_H
#define CONCEPTMAPCOMMANDSELECT_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "qtconceptmapcommand.h"
//#include "qtconceptmapmode.h"
#pragma GCC diagnostic pop

namespace ribi {
namespace cmap {

///Start a new node
///-Can be used only when there is an existing concept map
class CommandSelect final : public Command
{
  public:

  ///@param node_or_edge_name the node or edge name/text to be selected
  ///Will throw if name is absent (at redo)
  CommandSelect(
    QtConceptMap& qtconceptmap,
    const std::string& node_or_edge_name = ""
  );
  CommandSelect(const CommandSelect&) = delete;
  CommandSelect& operator=(const CommandSelect&) = delete;
  ~CommandSelect() noexcept {}

  ///Get the Node or Edge its name
  std::string GetName() const noexcept { return m_name; }
  void Redo() override;
  void Undo() override;

  private:
  /// If name was an edge name, the renamed edge
  QtEdge * m_renamed_qtedge;

  /// If name was a node name, the renamed node
  QtNode * m_renamed_qtnode;

  ///Text on the QtEdge or QtNode
  const std::string m_name;
};

/// Works on, for example  'create_new_node(0, 0, from)'
CommandSelect * ParseCommandSelect(
  QtConceptMap& qtconceptmap, std::string s);

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCOMMANDSELECT_H
