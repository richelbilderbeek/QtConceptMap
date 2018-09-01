#include "qtconceptmapcommand.h"

#include "qtconceptmap.h"

ribi::cmap::Command::Command(
  QtConceptMap& qtconceptmap,
  QUndoCommand * const parent
)
  : QUndoCommand(parent),
    m_qtconceptmap(qtconceptmap)
{

}

ribi::cmap::Command::~Command() noexcept
{

}

void ribi::cmap::Command::undo()
{
  CheckInvariants(GetQtConceptMap());
  Undo();
  CheckInvariants(GetQtConceptMap());
}

void ribi::cmap::Command::redo()
{
  CheckInvariants(GetQtConceptMap());
  Redo();
  CheckInvariants(GetQtConceptMap());
}

ribi::cmap::QtTool& ribi::cmap::GetQtToolItem(Command& cmd) noexcept
{
  return cmd.GetQtConceptMap().GetQtToolItem();
}

const ribi::cmap::QtTool& ribi::cmap::GetQtToolItem(const Command& cmd) noexcept
{
  return cmd.GetQtConceptMap().GetQtToolItem();
}

QGraphicsScene& ribi::cmap::GetScene(Command& cmd) noexcept
{
  return cmd.GetQtConceptMap().GetScene();
}

const QGraphicsScene& ribi::cmap::GetScene(const Command& cmd) noexcept
{
  return cmd.GetQtConceptMap().GetScene();
}
