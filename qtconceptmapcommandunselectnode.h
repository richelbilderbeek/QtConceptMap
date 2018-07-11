#ifndef CONCEPTMAPCOMMANDUNSELECTNODE_H
#define CONCEPTMAPCOMMANDUNSELECTNODE_H

//
//
//
//
#include "qtconceptmapcommand.h"
//

namespace ribi {
namespace cmap {

///Unselect a node additively
class CommandUnselectNode final : public Command
{
  public:

  ///@param node_or_edge_name the node or edge name/text to be selected
  ///Will throw if name is absent (at redo)
  CommandUnselectNode(
    QtConceptMap& qtconceptmap,
    QtNode * const qtnode,
    QUndoCommand * const parent = nullptr
  );
  CommandUnselectNode(const CommandUnselectNode&) = delete;
  CommandUnselectNode& operator=(const CommandUnselectNode&) = delete;
  ~CommandUnselectNode() noexcept {}

  void Redo() override;
  void Undo() override;

  private:

  /// Buddy of QtToolItem before redo
  QtNode * m_prev_qttoolitem_buddy;

  /// The QtNode
  QtNode * m_qtnode;

};

/// Works on, for example 'unselect_node(center)'
CommandUnselectNode * ParseCommandUnselectNode(
  QtConceptMap& qtconceptmap, std::string s);

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCOMMANDUNSELECTNODE_H
