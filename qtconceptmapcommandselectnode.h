#ifndef CONCEPTMAPCOMMANDSELECTNODE_H
#define CONCEPTMAPCOMMANDSELECTNODE_H

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
class CommandSelectNode final : public Command
{
  public:

  ///@param node_or_edge_name the node or edge name/text to be selected
  ///Will throw if name is absent (at redo)
  CommandSelectNode(
    QtConceptMap& qtconceptmap,
    QtNode * const qtnode
  );
  CommandSelectNode(const CommandSelectNode&) = delete;
  CommandSelectNode& operator=(const CommandSelectNode&) = delete;
  ~CommandSelectNode() noexcept {}

  void redo() override;
  void undo() override;

  private:

  /// The QtNode
  QtNode * m_qtnode;

};

/// Works on, for example 'select_node(center)'
CommandSelectNode * ParseCommandSelectNode(
  QtConceptMap& qtconceptmap, std::string s);

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCOMMANDSELECTNODE_H
