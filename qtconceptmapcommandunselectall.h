#ifndef CONCEPTMAPCOMMANDUNSELECTALL_H
#define CONCEPTMAPCOMMANDUNSELECTALL_H

#ifdef FIRST_FINISH_COMMAND_SELECT_ALL

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
#include "qtconceptmapfwd.h"
//#include "qtconceptmapmode.h"
#pragma GCC diagnostic pop


namespace ribi {
namespace cmap {

///Macro for UnselectEdge and UnselectNode for all selected QtEdges and QtNodes
class CommandUnselectAll final : public Command
{
  public:

  ///@param node_or_edge_name the node or edge name/text to be unselected
  CommandUnselectAll(
    QtConceptMap& qtconceptmap
  );
  CommandUnselectAll(const CommandUnselectAll&) = delete;
  CommandUnselectAll& operator=(const CommandUnselectAll&) = delete;
  ~CommandUnselectAll() noexcept {}

  void redo() override;
  void undo() override;

  private:

  //Nothing
};

/// Works on, for example  'unselect_all()'
CommandUnselectAll * ParseCommandUnselectAll(
  QtConceptMap& qtconceptmap, std::string s);

} //~namespace cmap
} //~namespace ribi


#endif // FIRST_FINISH_COMMAND_SELECT_ALL

#endif // CONCEPTMAPCOMMANDUNSELECTALL_H
