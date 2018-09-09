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
    qCritical() << __func__ << ": " << __LINE__ << ": " << "qtedge start";
    m_cmd = new CommandUnselectEdge(qtconceptmap, qtedge, this);
    qCritical() << __func__ << ": " << __LINE__ << ": " << "qtedge end";
  }
  else if (QtNode* const qtnode = qgraphicsitem_cast<QtNode*>(&item))
  {
    qCritical() << __func__ << ": " << __LINE__ << ": " << "qtnode start";
    if (IsOnEdge(*qtnode))
    {
      QtEdge * const qtedge2 = FindQtEdge(qtnode, qtconceptmap);
      assert(qtedge2);
      qCritical() << __func__ << ": " << __LINE__ << ": " << "new CommandUnselectEdge start";
      m_cmd = new CommandUnselectEdge(qtconceptmap, qtedge2, this);
      qCritical() << __func__ << ": " << __LINE__ << ": " << "new CommandUnselectEdge end";
    }
    else
    {
      qCritical() << __func__ << ": " << __LINE__ << ": " << "new CommandUnselectNode start";
      m_cmd = new CommandUnselectNode(qtconceptmap, qtnode, this);
      qCritical() << __func__ << ": " << __LINE__ << ": " << "new CommandUnselectNode end";
    }
    qCritical() << __func__ << ": " << __LINE__ << ": " << "qtnode end";
  }
  if (!m_cmd)
  {
    qCritical() << __func__ << ": " << __LINE__ << ": " << "no cmd";
    throw std::invalid_argument("item is not a QtEdge nor QtNode");
  }
  qCritical() << __func__ << ": " << __LINE__ << ": " << "start text";

  //QCommands have a text
  {
    std::stringstream msg;
    msg << "Unselect item";
    this->setText(msg.str().c_str());
  }
    qCritical() << __func__ << ": " << __LINE__ << ": " << "end text";
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
    if (qtnode && GetText(*qtnode) == t && !IsOnEdge(*qtnode))
    {
      return new CommandUnselect(qtconceptmap, *qtnode);
    }
  }
  return nullptr;
}

void ribi::cmap::CommandUnselect::Redo()
{
  assert(m_cmd);
  m_cmd->redo();
}

void ribi::cmap::CommandUnselect::Undo()
{
  assert(m_cmd);
  m_cmd->undo();
}
