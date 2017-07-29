#ifndef QTCONCEPTMAPCOMMANDTOGGLEARROWTAIL_H
#define QTCONCEPTMAPCOMMANDTOGGLEARROWTAIL_H

#include "conceptmap.h"
#include "conceptmapnode.h"
#include "conceptmapedge.h"
#include "qtconceptmapcommand.h"

struct QGraphicsScene;

namespace ribi {
namespace cmap {

struct QtEdge;
struct QtNode;

class CommandToggleArrowTail final : public Command
{
  public:

  CommandToggleArrowTail(
    QtConceptMap& qt4conceptmap
  );
  CommandToggleArrowTail(const CommandToggleArrowTail&) = delete;
  CommandToggleArrowTail& operator=(const CommandToggleArrowTail&) = delete;

  void redo() override;
  void undo() override;

  private:
  ConceptMap& m_conceptmap; //The concept map to modify
  const Edge m_edge_before;
  QGraphicsScene& m_scene;
  QtEdge * const m_qtedge; //The QtEdge to modify
};

} //~namespace cmap
} //~namespace ribi


#endif // QTCONCEPTMAPCOMMANDTOGGLEARROWTAIL_H
