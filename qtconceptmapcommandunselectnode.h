#ifndef CONCEPTMAPCOMMANDUNSELECTNODE_H
#define CONCEPTMAPCOMMANDUNSELECTNODE_H

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

  void redo() override;
  void undo() override;

  private:

  /// Buddy of QtExamplesItem before redo
  const QGraphicsItem * m_prev_qtexamplesitem_buddy;

  /// Buddy of QtToolItem before redo
  QtNode * m_prev_qttoolitem_buddy;

  /// The QtNode
  QtNode * m_qtnode;

};

/// Works on, for example 'select_node(center)'
CommandUnselectNode * ParseCommandUnselectNode(
  QtConceptMap& qtconceptmap, std::string s);

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCOMMANDUNSELECTNODE_H
