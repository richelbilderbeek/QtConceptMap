#ifndef CONCEPTMAPCOMMANDCREATENEWEDGE_H
#define CONCEPTMAPCOMMANDCREATENEWEDGE_H

#include "qtconceptmapcommand.h"

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


  void Redo() override;
  void Undo() override;

  ///Get the added QtEdge
  const std::string& GetText() const noexcept { return m_text; }

  private:
  ///The text to appear on the edge
  const std::string m_text;

  ///The QtEdge being added
  QtEdge * m_added_qtedge;

  ///Checks if added elements have already or not the supplied
  ///QScene. scene may be nullptr
  bool AllHaveScene(const QGraphicsScene * const scene) noexcept;

  ///Checks if redo can be run, will throw otherwise
  void CheckCanRedo() const;
};

///Add an Edge located in between two selected vertices with a certain text
EdgeDescriptor AddEdgeBetweenTwoSelectedVertices(
  const std::string& text,
  ConceptMap& c);

CommandCreateNewEdgeBetweenTwoSelectedNodes * ParseCommandCreateNewEdge(
  QtConceptMap& qtconceptmap, std::string s);

std::string GetText(const CommandCreateNewEdgeBetweenTwoSelectedNodes& c) noexcept;

///Get the mean X coordinat of the selected nodes
double GetSelectedNodesMeanX(const ConceptMap& c);

///Get the mean Y coordinat of the selected nodes
double GetSelectedNodesMeanY(const ConceptMap& c);

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCOMMANDCREATENEWEDGE_H
