#ifndef CONCEPTMAPCOMMANDUNSELECT_H
#define CONCEPTMAPCOMMANDUNSELECT_H

#include "qtconceptmapcommand.h"

namespace ribi {
namespace cmap {

///Picks the right command depending on a solitary QtNode or QtEdge
class CommandUnselect final : public Command
{
  public:

  ///@param node_or_edge_name the node or edge name/text to be unselected
  CommandUnselect(
    QtConceptMap& qtconceptmap,
    QGraphicsItem& item,
    QUndoCommand * const parent = nullptr
  );
  CommandUnselect(const CommandUnselect&) = delete;
  CommandUnselect& operator=(const CommandUnselect&) = delete;
  ~CommandUnselect() noexcept;

  void Redo() override;
  void Undo() override;

  private:

  ///The actual command
  Command * m_cmd;
};

/// Works on, for example  'create_new_node(0, 0, from)'
CommandUnselect * ParseCommandUnselect(
  QtConceptMap& qtconceptmap, std::string s);

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCOMMANDUNSELECT_H
