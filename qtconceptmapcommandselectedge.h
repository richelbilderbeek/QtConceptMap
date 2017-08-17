#ifndef CONCEPTMAPCOMMANDSELECTEDGE_H
#define CONCEPTMAPCOMMANDSELECTEDGE_H

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

/// Select a QtEdge
class CommandSelectEdge final : public Command
{
  public:

  ///@param node_or_edge_name the node or edge name/text to be selected
  ///Will throw if name is absent (at redo)
  CommandSelectEdge(
    QtConceptMap& qtconceptmap,
    QtEdge * const qtedge
  );
  CommandSelectEdge(const CommandSelectEdge&) = delete;
  CommandSelectEdge& operator=(const CommandSelectEdge&) = delete;
  ~CommandSelectEdge() noexcept {}

  const QtEdge * GetQtEdge() const noexcept { return m_qtedge; }

  ///Get the Node or Edge its name
  void redo() override;
  void undo() override;

  private:
  /// The selected QtEdge
  QtEdge * m_qtedge;
};

/// Works on, for example  'create_new_node(0, 0, from)'
CommandSelectEdge * ParseCommandSelectEdge(
  QtConceptMap& qtconceptmap, std::string s);

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCOMMANDSELECTEDGE_H