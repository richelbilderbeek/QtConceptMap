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
    if (!IsOnEdge(*qtnode))
    {
      m_cmd = new CommandUnselectNode(qtconceptmap, qtnode, this);
    }
    else
    {
      QtEdge * const qtedge2 = FindQtEdge(qtnode, qtconceptmap);
      assert(qtedge2);
      m_cmd = new CommandUnselectEdge(qtconceptmap, qtedge2, this);
    }
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
    if (QtEdge * const qtedge = qgraphicsitem_cast<QtEdge*>(item))
    {
      if (GetText(*qtedge) == t)
      {
        return new CommandUnselect(qtconceptmap, *qtedge);
      }
    }
    if (QtNode * const qtnode = qgraphicsitem_cast<QtNode*>(item))
    {
      if (GetText(*qtnode) == t)
      {
        return new CommandUnselect(qtconceptmap, *qtnode);
      }
    }
  }
  return nullptr;
}

void ribi::cmap::CommandUnselect::Redo()
{
  m_cmd->redo();
  /*
  m_unselected_qtnode = FindFirstQtNode(GetQtConceptMap().GetScene(),
    [name = m_name](QtNode * const qtnode)
    {
      return name == GetText(*qtnode);
    }
  );
  if (m_unselected_qtnode)
  {
    SetSelectedness(false, *m_unselected_qtnode, GetQtConceptMap());
  }
  else
  {
    m_unselected_qtedge = FindFirstQtEdge(GetQtConceptMap().GetScene(),
      [name = m_name](QtEdge * const qtedge)
      {
        return name == GetText(*qtedge);
      }
    );
    if (m_unselected_qtedge)
    {
      SetSelectedness(false, *m_unselected_qtedge, GetQtConceptMap());
    }
  }
  */
}

void ribi::cmap::CommandUnselect::Undo()
{
  m_cmd->undo();
  /*
  if (m_unselected_qtedge)
  {
    SetSelectedness(true, *m_unselected_qtedge, GetQtConceptMap());
  }
  if (m_unselected_qtnode)
  {
    SetSelectedness(true, *m_unselected_qtnode, GetQtConceptMap());
  }
  */
}
