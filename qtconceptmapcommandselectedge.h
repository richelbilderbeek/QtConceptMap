#ifndef CONCEPTMAPCOMMANDSELECTEDGE_H
#define CONCEPTMAPCOMMANDSELECTEDGE_H

#include "qtconceptmapcommand.h"

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
    QtEdge * const qtedge,
    QUndoCommand * const parent = nullptr
  );
  CommandSelectEdge(const CommandSelectEdge&) = delete;
  CommandSelectEdge& operator=(const CommandSelectEdge&) = delete;
  ~CommandSelectEdge() noexcept {}

  const QtEdge * GetQtEdge() const noexcept { return m_qtedge; }

  void Redo() override;
  void Undo() override;

  private:
  /// Buddy of QtToolItem before redo
  QtNode * m_prev_qttoolitem_buddy;

  /// The selected QtEdge
  QtEdge * m_qtedge;
};

/// Works on, for example 'select_edge(between)'
CommandSelectEdge * ParseCommandSelectEdge(
  QtConceptMap& qtconceptmap, std::string s);

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCOMMANDSELECTEDGE_H
