#include "qtconceptmapcommand.h"

#include "qtconceptmap.h"

ribi::cmap::Command::Command(QtConceptMap& qtconceptmap) noexcept
  : m_qtconceptmap(qtconceptmap)
{

}

ribi::cmap::ConceptMap& ribi::cmap::GetConceptMap(Command& cmd) noexcept
{
  return cmd.GetQtConceptMap().GetConceptMap();
}

const ribi::cmap::ConceptMap& ribi::cmap::GetConceptMap(const Command& cmd) noexcept
{
  return cmd.GetQtConceptMap().GetConceptMap();
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
