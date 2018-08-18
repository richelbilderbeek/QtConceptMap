#include "qtconceptmapcommandunselectall.h"

#include <cassert>
#include <boost/algorithm/string/trim.hpp>
#include <QDebug>

#include "qtconceptmap.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmapcommandunselect.h"

ribi::cmap::CommandUnselectAll::CommandUnselectAll(
  QtConceptMap& qtconceptmap,
  QUndoCommand *parent
)  : Command(qtconceptmap, parent),
     m_cmd{new QUndoCommand(this)}
{
  //Unselect the solitary QtNodes and QtEdges
  for (QGraphicsItem * const item: GetQtConceptMap().GetScene().selectedItems())
  {
    if (qgraphicsitem_cast<QtEdge*>(item)
      || (qgraphicsitem_cast<QtNode*>(item)
          && qgraphicsitem_cast<QtEdge*>(item->parentItem())
      )
    )
    {
      try
      {
        new CommandUnselect(GetQtConceptMap(), *item, m_cmd);
      }
      catch (const std::exception&) {} //!OCLINT indeed, an empty catch
    }
  }
  if (!m_cmd->childCount())
  {
    throw std::invalid_argument("Cannot unselect if none selected");
  }
  //QCommands have a text
  {
    std::stringstream msg;
    msg << "Unselect all (" << m_cmd->childCount() << " items)";
    this->setText(msg.str().c_str());
  }
}

ribi::cmap::CommandUnselectAll::~CommandUnselectAll() noexcept
{

}

ribi::cmap::CommandUnselectAll * ribi::cmap::ParseCommandUnselectAll(
  QtConceptMap& qtconceptmap, std::string s)
{
  //"unselect_all()"
  boost::algorithm::trim(s);
  const std::string str_begin = "unselect_all(";
  if (s.substr(0, str_begin.size()) != str_begin) return nullptr;
  if (s.back() != ')') return nullptr;
  //""
  const std::string t = s.substr(str_begin.size(), s.size() - str_begin.size() - 1);
  //assert(t[0] != '(');
  //assert(t.back() != ')');
  if (!t.empty()) return nullptr;
  return new CommandUnselectAll(qtconceptmap);
}

void ribi::cmap::CommandUnselectAll::Redo()
{
  assert(m_cmd);
  m_cmd->redo();
}

void ribi::cmap::CommandUnselectAll::Undo()
{
  m_cmd->undo();
}
