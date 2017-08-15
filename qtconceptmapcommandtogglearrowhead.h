#ifndef QTCONCEPTMAPCOMMANDTOGGLEARROWHEAD_H
#define QTCONCEPTMAPCOMMANDTOGGLEARROWHEAD_H

#include "conceptmap.h"
#include "conceptmapnode.h"
#include "conceptmapedge.h"
#include "qtconceptmapcommand.h"

struct QGraphicsScene;

namespace ribi {
namespace cmap {

struct QtEdge;
struct QtNode;

///Toggles the head of a selected QtEdge
class CommandToggleArrowHead final : public Command
{
  public:

  CommandToggleArrowHead(
    QtConceptMap& qtconceptmap
  );
  CommandToggleArrowHead(const CommandToggleArrowHead&) = delete;
  CommandToggleArrowHead& operator=(const CommandToggleArrowHead&) = delete;

  void redo() override;
  void undo() override;

  private:


  ConceptMap& m_conceptmap; //The concept map to modify

  const Edge m_edge_before;
  QGraphicsScene& m_scene;
  QtEdge * const m_qtedge; //The QtEdge to modify
};

using CommandToggleHead = CommandToggleArrowHead;

CommandToggleArrowHead * ParseCommandToggleArrowHead(
  QtConceptMap& qtconceptmap, std::string s);

inline CommandToggleHead * ParseCommandToggleHead(
  QtConceptMap& q, std::string s) { return ParseCommandToggleArrowHead(q, s); }


} //~namespace cmap
} //~namespace ribi


#endif // QTCONCEPTMAPCOMMANDTOGGLEARROWHEAD_H
