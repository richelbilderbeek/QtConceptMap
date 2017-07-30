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
  const std::string& GetText() const noexcept { return m_text; }

  private:
  QtConceptMap& m_qtconceptmap;

  ///The text to appear on the edge
  const std::string m_text;

  ///The QtEdge being added
  QtEdge * m_added_qtedge;

  ///The QtNode being at the center of m_added_qtedge
  QtNode * m_added_qtnode;

  ///The first selected QtNode
  QtNode * m_qtfrom;

  ///The second selected QtNode
  QtNode * m_qtto;

  ///All selected QGraphicsItem before the command
  QList<QGraphicsItem *> m_selected_before;

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
