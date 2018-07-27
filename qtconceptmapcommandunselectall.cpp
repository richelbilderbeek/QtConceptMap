#include "qtconceptmapcommandunselectall.h"

#include <cassert>
#include <boost/algorithm/string/trim.hpp>
#include <gsl/gsl_assert>
#include "count_vertices_with_selectedness.h"
#include "container.h"
#include "find_first_custom_vertex_with_my_vertex.h"
#include "find_first_custom_edge_with_my_edge.h"
#include "set_edge_selectedness.h"
#include "set_vertex_selectedness.h"
#include "conceptmap.h"
#include "conceptmaphelper.h"
#include "conceptmapnode.h"
#include "qtconceptmap.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmaphelper.h"
#include "qtconceptmapcommandunselect.h"
#include <QGraphicsScene>

ribi::cmap::CommandUnselectAll::CommandUnselectAll(
  QtConceptMap& qtconceptmap,
  QUndoCommand *parent
)  : Command(qtconceptmap, parent),
     m_cmds{}
{
  for (QGraphicsItem * const item: GetQtConceptMap().GetScene().items())
  {
    assert(item);
    if (!item->isSelected()) continue;

    //Do not add a QtNode that is on a QtEdge, only keep that QtEdge
    if (QtNode * const qtnode = qgraphicsitem_cast<QtNode*>(item))
    {
      if (FindQtEdge(qtnode, GetQtConceptMap())) continue;
    }

    try
    {

      assert(qgraphicsitem_cast<QtEdge*>(item) || qgraphicsitem_cast<QtNode*>(item));
      m_cmds.push_back(new CommandUnselect(GetQtConceptMap(), *item, this));
    }
    catch (std::exception&) {} //OK
  }
  if (m_cmds.empty())
  {
    throw std::invalid_argument("Cannot unselect if none selected");
  }
  /*
  if (QtEdge* const qtedge = qgraphicsitem_cast<QtEdge*>(&item))
  {
    m_cmds = new CommandUnselectAllEdge(qtconceptmap, qtedge, this);
  }
  else if (QtNode* const qtnode = qgraphicsitem_cast<QtNode*>(&item))
  {
    if (QtEdge* const qtedge2 = FindQtEdge(qtnode, GetQtConceptMap()))
    {
      m_cmds = new CommandUnselectAllEdge(qtconceptmap, qtedge2, this);
    }
    else
    {
      m_cmds = new CommandUnselectAllNode(qtconceptmap, qtnode, this);
    }
  }
  if (!m_cmds)
  {
    throw std::invalid_argument("item is not a QtEdge nor QtNode");
  }
  */
  //QCommands have a text
  {
    std::stringstream msg;
    msg << "Unselect all";
    this->setText(msg.str().c_str());
  }
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
  for (auto * const cmd: m_cmds)
  {
    cmd->redo();
  }
  /*
  m_unselectalled_qtnode = FindFirstQtNode(GetQtConceptMap().GetScene(),
    [name = m_name](QtNode * const qtnode)
    {
      return name == GetText(*qtnode);
    }
  );
  if (m_unselectalled_qtnode)
  {
    SetSelectedness(false, *m_unselectalled_qtnode, GetQtConceptMap());
  }
  else
  {
    m_unselectalled_qtedge = FindFirstQtEdge(GetQtConceptMap().GetScene(),
      [name = m_name](QtEdge * const qtedge)
      {
        return name == GetText(*qtedge);
      }
    );
    if (m_unselectalled_qtedge)
    {
      SetSelectedness(false, *m_unselectalled_qtedge, GetQtConceptMap());
    }
  }
  */
}

void ribi::cmap::CommandUnselectAll::Undo()
{
  std::for_each(
    std::rbegin(m_cmds), std::rend(m_cmds),
    [](Command * const cmd)
    {
      cmd->undo();
    }
  );
  /*
  if (m_unselectalled_qtedge)
  {
    SetSelectedness(true, *m_unselectalled_qtedge, GetQtConceptMap());
  }
  if (m_unselectalled_qtnode)
  {
    SetSelectedness(true, *m_unselectalled_qtnode, GetQtConceptMap());
  }
  */
}
