#ifndef CONCEPTMAPCOMMAND_H
#define CONCEPTMAPCOMMAND_H

#include <string>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <QUndoCommand>
#include "conceptmapfwd.h"
#pragma GCC diagnostic pop

namespace ribi {
namespace cmap {

///Command can be used to do and undo commands to a concept map Widget
///Command must use a Widget* because a Widget will call a Command with this:
///
///  some_command->CanDo(this);
///
struct Command : public QUndoCommand
{
  Command() noexcept {}
  virtual ~Command() noexcept {}

  virtual void undo() = 0;
  virtual void redo() = 0;
};

} //~namespace cmap
} //~namespace ribi


#endif // CONCEPTMAPCOMMAND_H
