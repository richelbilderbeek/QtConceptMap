#ifndef CONCEPTMAPCOMMANDSELECTNODE_H
#define CONCEPTMAPCOMMANDSELECTNODE_H

#include "qtconceptmapcommand.h"

namespace ribi {
namespace cmap {

///Select a node additively
class CommandSelectNode final : public Command
{
  public:

  ///@param node_or_edge_name the node or edge name/text to be selected
  ///Will throw if name is absent (at redo)
  CommandSelectNode(
    QtConceptMap& qtconceptmap,
    QtNode * const qtnode,
    QUndoCommand * const parent = nullptr
  );
  CommandSelectNode(const CommandSelectNode&) = delete;
  CommandSelectNode& operator=(const CommandSelectNode&) = delete;
  ~CommandSelectNode() noexcept;

  void Redo() override;
  void Undo() override;

  private:

  /// Buddy of QtToolItem before redo, may be nullptr
  QtNode * m_prev_qttoolitem_buddy;

  /// The QtNode selected, will never be null
  QtNode * m_qtnode;

};

/// Works on, for example 'select_node(center)'
CommandSelectNode * ParseCommandSelectNode(
  QtConceptMap& qtconceptmap, std::string s);

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCOMMANDSELECTNODE_H
