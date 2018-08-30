#ifndef CONCEPTMAPCOMMANDDELETENODE_H
#define CONCEPTMAPCOMMANDDELETENODE_H

#include "qtconceptmapcommand.h"

namespace ribi {
namespace cmap {

///Delete all selecteded edges and nodes, except the center node
class CommandDeleteSelected final : public Command
{
  public:
  CommandDeleteSelected(
    QtConceptMap& qtconceptmap
  );
  CommandDeleteSelected(const CommandDeleteSelected&) = delete;
  CommandDeleteSelected& operator=(const CommandDeleteSelected&) = delete;
  ~CommandDeleteSelected() noexcept;

  void Undo() override;
  void Redo() override;

  private:
  //ConceptMap m_conceptmap_after;
  //const ConceptMap m_conceptmap_before;
  QGraphicsItem * const m_focus_item_before;

  ///The QtEdges that were selected when removed
  std::vector<QtEdge *> m_selected_qtedges_removed;
  std::vector<QtEdge *> m_unselected_qtedges_removed;

  std::vector<QtNode *> m_qtnodes_removed;

  QtNode * const m_tool_item_old_buddy;

  void AddDeletedQtEdges();
  void AddDeletedQtNodes();

  ///All items in m_qtedges_removed and m_qtnodes_removed must all have
  /// either (1) no scene, or (2) the same scene
  bool AllHaveSameScene() const noexcept;

  ///Remove the selected QtEdges and the QtEdges connected to a selected QtNode
  void RemoveQtEdges();

  ///Remove the selected QtNpodes
  void RemoveSelectedQtNodes();

  ///Each QtEdge that is on a QtNode that is selected to be deleted,
  ///must be selected, so it will be deleted as well
  //void SelectAllQtEdgesOnSelectedQtNodes();

  void SetSelected(const QList<QGraphicsItem *>& v, const bool is_selected) noexcept;

};

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCOMMANDDELETENODE_H
