#ifndef CONCEPTMAPCOMMAND_H
#define CONCEPTMAPCOMMAND_H

#include <string>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <QUndoCommand>
#include "conceptmapgraphtypes.h"
#include "conceptmapfwd.h"
#include "qtconceptmapfwd.h"
#pragma GCC diagnostic pop

namespace ribi {
namespace cmap {

///Command can be used to do and undo commands to a QtConceptMap
struct Command : public QUndoCommand
{
  Command(QtConceptMap& qtconceptmap) noexcept;
  virtual ~Command() noexcept {}

  auto& GetQtConceptMap() noexcept { return m_qtconceptmap; }
  const auto& GetQtConceptMap() const noexcept { return m_qtconceptmap; }

  virtual void undo() = 0;
  virtual void redo() = 0;

  private:
  QtConceptMap& m_qtconceptmap;
};

//Convenience
ConceptMap& GetConceptMap(Command& cmd) noexcept;

//Convenience
const ConceptMap& GetConceptMap(const Command& cmd) noexcept;

//Convenience
QtTool& GetQtToolItem(Command& cmd) noexcept;

//Convenience
const QtTool& GetQtToolItem(const Command& cmd) noexcept;

//Convenience
QGraphicsScene& GetScene(Command& cmd) noexcept;

//Convenience
const QGraphicsScene& GetScene(const Command& cmd) noexcept;

} //~namespace cmap
} //~namespace ribi


#endif // CONCEPTMAPCOMMAND_H
