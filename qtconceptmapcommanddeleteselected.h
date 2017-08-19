#ifndef CONCEPTMAPCOMMANDDELETENODE_H
#define CONCEPTMAPCOMMANDDELETENODE_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"

#include "conceptmapnode.h"
#include "conceptmap.h"
#include "qtconceptmapcommand.h"
#include <QGraphicsScene>
#include "qtconceptmaptoolsitem.h"
#include <vector>
#pragma GCC diagnostic pop

namespace ribi {
namespace cmap {

///Delete all selecteded edges and node
class CommandDeleteSelected final : public Command
{
  public:
  CommandDeleteSelected(
    QtConceptMap& qtconceptmap
  );
  CommandDeleteSelected(const CommandDeleteSelected&) = delete;
  CommandDeleteSelected& operator=(const CommandDeleteSelected&) = delete;
  ~CommandDeleteSelected() noexcept {}

  void Undo() override;
  void Redo() override;

  private:
  ConceptMap& m_conceptmap;
  //ConceptMap m_conceptmap_after;
  //const ConceptMap m_conceptmap_before;
  QGraphicsItem * const m_focus_item_before;
  std::vector<QtEdge *> m_qtedges_removed;
  std::vector<QtNode *> m_qtnodes_removed;

  //QGraphicsScene& m_scene;
  const QList<QGraphicsItem *> m_selected_before;
  //QtTool& m_tool_item;
  QtNode * const m_tool_item_old_buddy;

  void AddDeletedQtEdges();
  void AddDeletedQtNodes();

  ///All items in m_qtedges_removed and m_qtnodes_removed must all have
  /// either (1) no scene, or (2) the same scene
  bool AllHaveSameScene() const noexcept;


  ///Checks if the QtNodes are in a valid state before deletion
  void CheckQtNodesBeforeDelete() const noexcept;

  ///Remove the selected QtEdges
  void RemoveSelectedQtEdges();

  ///Remove the selected QtNpodes
  void RemoveSelectedQtNodes();

  ///Each QtEdge that is on a QtNode that is selected to be deleted,
  ///must be selected, so it will be deleted as well
  void SelectAllQtEdgesOnSelectedQtNodes();

  void SetSelected(const QList<QGraphicsItem *>& v, const bool is_selected) noexcept;

};

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCOMMANDDELETENODE_H
