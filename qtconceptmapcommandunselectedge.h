#ifndef CONCEPTMAPCOMMANDUNSELECTEDGE_H
#define CONCEPTMAPCOMMANDUNSELECTEDGE_H

//
//
//
//
#include "qtconceptmapcommand.h"
//

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
    QtEdge * const qtedge,
    QUndoCommand * const parent = nullptr
  );
  CommandUnselectEdge(const CommandUnselectEdge&) = delete;
  CommandUnselectEdge& operator=(const CommandUnselectEdge&) = delete;
  ~CommandUnselectEdge() noexcept {}

  void Redo() override;
  void Undo() override;

  private:

  /// Buddy of QtToolItem before redo
  QtNode * m_prev_qtexamplesitem_buddy;

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
