#ifndef CONCEPTMAPCOMMANDSETCONCEPT_H
#define CONCEPTMAPCOMMANDSETCONCEPT_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"

#include "conceptmapnode.h"
#include "conceptmap.h"
#include "qtconceptmapqtnode.h"
#include <QGraphicsScene>
#include "qtconceptmaptoolsitem.h"
#include "qtconceptmapcommand.h"
#include "qtconceptmapmode.h"
#pragma GCC diagnostic pop

namespace ribi {
namespace cmap {

///Select a node additively
class CommandSetConcept final : public Command
{
  public:

  ///@param node_or_edge_name the node or edge name/text to be selected
  ///Will throw if name is absent (at redo)
  CommandSetConcept(
    QtConceptMap& qtconceptmap,
    const Concept& concept
  );
  CommandSetConcept(const CommandSetConcept&) = delete;
  CommandSetConcept& operator=(const CommandSetConcept&) = delete;
  ~CommandSetConcept() noexcept {}

  void redo() override;
  void undo() override;

  private:

  ///The new Concept
  const Concept m_concept;

  ///The Concept before redo; the Concept after undo
  Concept m_prev_concept;

};

/// Works on, for example 'set_concept(<concept xml>)'
CommandSetConcept * ParseCommandSetConcept(
  QtConceptMap& qtconceptmap, std::string s);

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCOMMANDSETCONCEPT_H
