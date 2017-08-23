#ifndef CONCEPTMAPCOMMANDUNSELECTNODE_H
#define CONCEPTMAPCOMMANDUNSELECTNODE_H

//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Weffc++"
//#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
//#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "qtconceptmapcommand.h"
//#pragma GCC diagnostic pop

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
    QtNode * const qtnode
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
