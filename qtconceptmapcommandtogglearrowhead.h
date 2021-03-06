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

  void Redo() override;
  void Undo() override;

  private:

  //Nothing
};

using CommandToggleHead = CommandToggleArrowHead;

CommandToggleArrowHead * ParseCommandToggleArrowHead(
  QtConceptMap& qtconceptmap, std::string s);

inline CommandToggleHead * ParseCommandToggleHead(
  QtConceptMap& q, std::string s) { return ParseCommandToggleArrowHead(q, s); }


} //~namespace cmap
} //~namespace ribi


#endif // QTCONCEPTMAPCOMMANDTOGGLEARROWHEAD_H
