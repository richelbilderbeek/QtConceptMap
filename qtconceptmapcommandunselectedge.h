#ifndef CONCEPTMAPCOMMANDUNSELECTEDGE_H
#define CONCEPTMAPCOMMANDUNSELECTEDGE_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
//#include "conceptmapnode.h"
//#include "conceptmap.h"
//#include "qtconceptmapqtnode.h"
//#include <QGraphicsScene>
//#include "qtconceptmaptoolsitem.h"
#include "qtconceptmapcommand.h"
//#include "qtconceptmapmode.h"
#pragma GCC diagnostic pop

namespace ribi {
namespace cmap {

///Unselect a node additively
class CommandUnselectEdge final : public Command
{
  public:

  ///@param node_or_edge_name the node or edge name/text to be selected
  ///Will throw if name is absent (at redo)
  CommandUnselectEdge(
    QtConceptMap& qtconceptmap,
    QtEdge * const qtedge
  );
  CommandUnselectEdge(const CommandUnselectEdge&) = delete;
  CommandUnselectEdge& operator=(const CommandUnselectEdge&) = delete;
  ~CommandUnselectEdge() noexcept {}

  void Redo() override;
  void Undo() override;

  private:

  /// Buddy of QtToolItem before redo
  QtNode * m_prev_qttoolitem_buddy;

  /// The QtEdge
  QtEdge * m_qtedge;

};

/// Works on, for example 'unselect_node(center)'
CommandUnselectEdge * ParseCommandUnselectEdge(
  QtConceptMap& qtconceptmap, std::string s);

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCOMMANDUNSELECTEDGE_H
