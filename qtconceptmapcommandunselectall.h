#ifndef CONCEPTMAPCOMMANDUNSELECTALL_H
#define CONCEPTMAPCOMMANDUNSELECTALL_H

#include "qtconceptmapcommand.h"

namespace ribi {
namespace cmap {

///Unselects all
///Will throw is there is nothing to unselect
class CommandUnselectAll final : public Command
{
  public:

  ///@param node_or_edge_name the node or edge name/text to be unselected
  CommandUnselectAll(
    QtConceptMap& qtconceptmap,
    QUndoCommand *parent = nullptr
  );
  CommandUnselectAll(const CommandUnselectAll&) = delete;
  CommandUnselectAll& operator=(const CommandUnselectAll&) = delete;
  ~CommandUnselectAll() noexcept;

  void Redo() override;
  void Undo() override;

  private:

  ///The actual command, in which subcommands are children
  QUndoCommand * m_cmd;
};

/// Works on, for example  'create_new_node(0, 0, from)'
CommandUnselectAll * ParseCommandUnselectAll(
  QtConceptMap& qtconceptmap, std::string s);

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCOMMANDUNSELECTALL_H
