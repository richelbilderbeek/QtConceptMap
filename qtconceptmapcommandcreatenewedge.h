#ifndef CONCEPTMAPCOMMANDCREATENEWEDGE_H
#define CONCEPTMAPCOMMANDCREATENEWEDGE_H

#include "conceptmap.h"
#include "conceptmapnode.h"
#include "conceptmapedge.h"
#include "qtconceptmapcommand.h"
#include "qtconceptmapmode.h"

struct QGraphicsItem;
struct QGraphicsScene;

namespace ribi {
namespace cmap {

struct QtEdge;
struct QtNode;
struct QtTool;

class CommandCreateNewEdgeBetweenTwoSelectedNodes final : public Command
{
  public:

  CommandCreateNewEdgeBetweenTwoSelectedNodes(
    QtConceptMap& qtconceptmap,
    const std::string& text = ""
  );
  CommandCreateNewEdgeBetweenTwoSelectedNodes(
    const CommandCreateNewEdgeBetweenTwoSelectedNodes&
  ) = delete;
  CommandCreateNewEdgeBetweenTwoSelectedNodes& operator=(
    const CommandCreateNewEdgeBetweenTwoSelectedNodes&
  ) = delete;
  ~CommandCreateNewEdgeBetweenTwoSelectedNodes() noexcept;


  void redo() override;
  void undo() override;

  ///Get the added QtEdge
  const QtEdge& GetQtEdge() const noexcept { return *m_added_qtedge; }

  private:
  ConceptMap& m_conceptmap;
  Edge m_added_edge; //The Edge being added or removed
  QtEdge * m_added_qtedge; //The QtEdge being added or removed
  QtNode * m_added_qtnode; //The QtNode being at the center of m_qtedge
  ConceptMap m_after;
  const ConceptMap m_before;
  const Mode m_mode;
  QGraphicsScene& m_scene;
  const QList<QGraphicsItem *> m_selected_before;
  QtTool& m_tool_item;

  ///Checks if added elements have already or not the supplied
  ///QScene. scene may be nullptr
  bool AllHaveScene(const QGraphicsScene * const scene) noexcept;
};

CommandCreateNewEdgeBetweenTwoSelectedNodes * parse_command_create_new_edge(
  QtConceptMap& qtconceptmap, std::string s);

std::string GetText(const CommandCreateNewEdgeBetweenTwoSelectedNodes& c) noexcept;

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCOMMANDCREATENEWEDGE_H
