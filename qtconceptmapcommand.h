#ifndef CONCEPTMAPCOMMAND_H
#define CONCEPTMAPCOMMAND_H

#include <QUndoCommand>
#include "conceptmapgraphtypes.h"
#include "conceptmapfwd.h"
#include "qtconceptmapfwd.h"

namespace ribi {
namespace cmap {

///Command can be used to do and undo commands to a QtConceptMap
struct Command : public QUndoCommand
{
  Command(
    QtConceptMap& qtconceptmap,
    QUndoCommand * const parent = nullptr
  ) noexcept;
  virtual ~Command() noexcept;

  auto& GetQtConceptMap() noexcept { return m_qtconceptmap; }
  const auto& GetQtConceptMap() const noexcept { return m_qtconceptmap; }

  ///Template method, that checks QtConceptMap its invariants,
  ///before and after calling the virtual function Undo
  void undo() final override;

  ///Template method, that checks QtConceptMap its invariants,
  ///before and after calling the virtual function Redo
  void redo() final override;

  private:
  QtConceptMap& m_qtconceptmap;

  ///Actual implementation of undo
  virtual void Undo() = 0;

  ///Actual implementation of undo
  virtual void Redo() = 0;
};

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
