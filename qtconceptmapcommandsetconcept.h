#ifndef CONCEPTMAPCOMMANDSETCONCEPT_H
#define CONCEPTMAPCOMMANDSETCONCEPT_H

//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Weffc++"
//#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
//#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
//#include "conceptmapnode.h"
//#include "conceptmap.h"
//#include "qtconceptmapqtnode.h"
//#include <QGraphicsScene>
//#include "qtconceptmaptoolsitem.h"
#include "qtconceptmapcommand.h"
//#include "qtconceptmapmode.h"
//#pragma GCC diagnostic pop

namespace ribi {
namespace cmap {

///Set the Concept of the QtNode (also on QtEdge) where the QtToolItem
///is connected to. Will throw if there is not QtToolItem or if it
///is connected to an uneditable center QtNode
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

  const Concept& GetConcept() const noexcept { return m_concept; }
  const Concept& GetPrevConcept() const noexcept { return m_prev_concept; }

  void Redo() override;

  void SetPrevConcept(const Concept& concept) { m_prev_concept = concept; }

  void Undo() override;

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
