#ifndef CONCEPTMAPCOMMANDSETCONCEPT_H
#define CONCEPTMAPCOMMANDSETCONCEPT_H

#include "qtconceptmapcommand.h"

namespace ribi {
namespace cmap {

///Set the Concept of the single one QtNode (also on QtEdge) selected.
///Will throw if there are no or multiple QtNodes selected, or the
///selected item is uneditable
class CommandSetConcept final : public Command
{
  public:

  CommandSetConcept(
    QtConceptMap& qtconceptmap,
    const Concept& concept
  );
  CommandSetConcept(const CommandSetConcept&) = delete;
  CommandSetConcept& operator=(const CommandSetConcept&) = delete;
  ~CommandSetConcept() noexcept;

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
