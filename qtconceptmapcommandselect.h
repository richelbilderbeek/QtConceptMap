#ifndef CONCEPTMAPCOMMANDSELECT_H
#define CONCEPTMAPCOMMANDSELECT_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "qtconceptmapcommand.h"
#pragma GCC diagnostic pop

namespace ribi {
namespace cmap {

///Start a new node
///-Can be used only when there is an existing concept map
class CommandSelect final : public Command
{
  public:

  ///@param item a QtEdge or QtNode
  ///Will throw if item is not a QtEdge nor QtNode
  CommandSelect(
    QtConceptMap& qtconceptmap,
    QGraphicsItem& item
  );
  CommandSelect(const CommandSelect&) = delete;
  CommandSelect& operator=(const CommandSelect&) = delete;
  ~CommandSelect() noexcept {}

  ///Get the Node or Edge its name
  //std::string GetName() const noexcept { return m_name; }
  void Redo() override;
  void Undo() override;

  private:
  /// If name was an edge name, the renamed edge
  //QtEdge * m_renamed_qtedge;
  //QGraphicsItem * const m_item;
  Command * m_cmd;

  /// If name was a node name, the renamed node
  //QtNode * m_renamed_qtnode;

  ///Text on the QtEdge or QtNode
  //const std::string m_name;
};

/// Works on, for example  'select(from)'
CommandSelect * ParseCommandSelect(
  QtConceptMap& qtconceptmap, std::string s);

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCOMMANDSELECT_H
