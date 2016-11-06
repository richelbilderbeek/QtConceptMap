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
#include <set>
#pragma GCC diagnostic pop

namespace ribi {
namespace cmap {

///Delete all selecteded edges and node
class CommandDeleteSelected final : public Command
{
  public:
  CommandDeleteSelected(
    ConceptMap& conceptmap,
    QGraphicsScene& scene,
    QtTool& tool_item
  );
  CommandDeleteSelected(const CommandDeleteSelected&) = delete;
  CommandDeleteSelected& operator=(const CommandDeleteSelected&) = delete;
  ~CommandDeleteSelected() noexcept {}

  void undo() override;
  void redo() override;

  private:
  ConceptMap& m_conceptmap;
  ConceptMap m_conceptmap_after;
  const ConceptMap m_conceptmap_before;
  QGraphicsItem * const m_focus_item_before;
  std::set<QtEdge *> m_qtedges_removed;
  std::set<QtNode *> m_qtnodes_removed;

  QGraphicsScene& m_scene;
  const QList<QGraphicsItem *> m_selected_before;
  QtTool& m_tool_item;
  QtNode * const m_tool_item_old_buddy;

  void AddToScene(const std::set<QtEdge *>& qtedges);
  void AddToScene(const std::set<QtNode *>& qtnodes);

  ///All items in m_qtedges_removed and m_qtnodes_removed must all have
  /// either (1) no scene, or (2) the same scene
  bool AllHaveSameScene() const noexcept;


  ///Checks if the QtNodes are in a valid state before deletion
  void CheckQtNodesBeforeDelete() const noexcept;

  ///Collect the QtEdges to be removed, must be called after CollectQtNodesRemoved
  void CollectQtEdgesRemoved();

  ///Collect the QtNodes to be removed
  void CollectQtNodesRemoved();

  void SetSelected(const QList<QGraphicsItem *>& v, const bool is_selected) const noexcept;

};

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCOMMANDDELETENODE_H
