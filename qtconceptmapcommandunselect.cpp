#include "qtconceptmapcommandunselect.h"

#include <cassert>

#include <boost/algorithm/string/trim.hpp>

#include <QDebug>
#include <QUndoCommand>

#include "qtconceptmap.h"
#include "qtconceptmapcommandunselectedge.h"
#include "qtconceptmapcommandunselectnode.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmapqtnode.h"

ribi::cmap::CommandUnselect::CommandUnselect(
  QtConceptMap& qtconceptmap,
  QGraphicsItem& item,
  QUndoCommand * const parent
)  : Command(qtconceptmap, parent),
     m_cmd{nullptr}
{
  if (QtEdge* const qtedge = qgraphicsitem_cast<QtEdge*>(&item))
  {
    m_cmd = new CommandUnselectEdge(qtconceptmap, qtedge, this);
  }
  else if (QtNode* const qtnode = qgraphicsitem_cast<QtNode*>(&item))
  {
    m_cmd = new CommandUnselectNode(qtconceptmap, qtnode, this);
    assert(!IsOnEdge(*qtnode));
    #ifdef KEEP_20180902
    if (IsOnEdge(*qtnode))
    {
      QtEdge * const qtedge2 = FindQtEdge(qtnode, qtconceptmap);
      assert(qtedge2);
      m_cmd = new CommandUnselectEdge(qtconceptmap, qtedge2, this);
    }
    else
    {
      m_cmd = new CommandUnselectNode(qtconceptmap, qtnode, this);
    }
    #endif // KEEP_20180902
  }
  if (!m_cmd)
  {
    throw std::invalid_argument("item is not a QtEdge nor QtNode");
  }

  //QCommands have a text
  {
    std::stringstream msg;
    msg << "Unselect item";
    this->setText(msg.str().c_str());
  }
}

ribi::cmap::CommandUnselect::~CommandUnselect() noexcept
{

}

ribi::cmap::CommandUnselect * ribi::cmap::ParseCommandUnselect(
  QtConceptMap& qtconceptmap, std::string s)
{
  //"select(my text)"
  boost::algorithm::trim(s);
  const std::string str_begin = "unselect(";
  if (s.substr(0, str_begin.size()) != str_begin) return nullptr;
  if (s.back() != ')') return nullptr;
  //"my text"
  const std::string t = s.substr(str_begin.size(), s.size() - str_begin.size() - 1);
  assert(t[0] != '(');
  assert(t.back() != ')');
  for (QGraphicsItem * const item: qtconceptmap.items())
  {
    QtEdge * const qtedge = qgraphicsitem_cast<QtEdge*>(item);
    if (qtedge && GetText(*qtedge) == t)
    {
      return new CommandUnselect(qtconceptmap, *qtedge);
    }
    QtNode * const qtnode = qgraphicsitem_cast<QtNode*>(item);
    if (qtnode && !IsOnEdge(*qtnode) && GetText(*qtnode) == t)
    {
      return new CommandUnselect(qtconceptmap, *qtnode);
    }
  }
  return nullptr;
}

void ribi::cmap::CommandUnselect::Redo()
{
  m_cmd->redo();
}

void ribi::cmap::CommandUnselect::Undo()
{
  m_cmd->undo();
}
