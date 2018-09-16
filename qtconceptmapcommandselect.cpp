#include "qtconceptmapcommandselect.h"

#include <cassert>
#include <boost/algorithm/string/trim.hpp>
#include <gsl/gsl_assert>

#include <QDebug>

#include "count_vertices_with_selectedness.h"
#include "container.h"
#include "conceptmap.h"
#include "conceptmaphelper.h"
#include "conceptmapnode.h"
#include "qtconceptmap.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmaphelper.h"
#include "qtconceptmapcommandselectedge.h"
#include "qtconceptmapcommandselectnode.h"

ribi::cmap::CommandSelect::CommandSelect(
  QtConceptMap& qtconceptmap,
  QGraphicsItem& item
)
  : Command(qtconceptmap),
    m_cmd{nullptr}
{
  try
  {
    if (QtEdge* const qtedge = qgraphicsitem_cast<QtEdge*>(&item))
    {
      m_cmd = new CommandSelectEdge(qtconceptmap, qtedge, this);
    }
    else if (QtNode* const qtnode = qgraphicsitem_cast<QtNode*>(&item))
    {
      if (QtEdge * const qtedge2 = FindQtEdge(qtnode, GetQtConceptMap()))
      {
        m_cmd = new CommandSelectEdge(qtconceptmap, qtedge2, this);
      }
      else
      {
        m_cmd = new CommandSelectNode(qtconceptmap, qtnode, this);
      }
    }
  }
  catch (const std::exception& e)
  {
    throw e;
  }
  if (!m_cmd)
  {
    throw std::invalid_argument("item is not a QtEdge nor QtNode");
  }

  //QCommands have a text
  {
    assert(m_cmd);
    this->setText(m_cmd->text());
  }
}

ribi::cmap::CommandSelect::~CommandSelect() noexcept
{

}

ribi::cmap::CommandSelect * ribi::cmap::ParseCommandSelect(
  QtConceptMap& qtconceptmap, std::string s)
{
  //"select(my text)"
  boost::algorithm::trim(s);
  const std::string str_begin = "select(";
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
        return new CommandSelect(qtconceptmap, *qtedge);
      }
    }
    if (QtNode * const qtnode = qgraphicsitem_cast<QtNode*>(item))
    {
      if (QtEdge * const qtedge2 = FindQtEdge(qtnode, qtconceptmap))
      {
        if (GetText(*qtedge2) == t)
        {
          return new CommandSelect(qtconceptmap, *qtedge2);
        }
      }
      else
      {
        if (GetText(*qtnode) == t)
        {
          return new CommandSelect(qtconceptmap, *qtnode);
        }
      }
    }
  }
  return nullptr;
}

void ribi::cmap::CommandSelect::Redo()
{
  assert(m_cmd);
  m_cmd->redo();
}

void ribi::cmap::CommandSelect::Undo()
{
  assert(m_cmd);
  m_cmd->undo();
}
