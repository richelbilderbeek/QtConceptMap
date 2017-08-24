#include "qtconceptmapcommandunselect.h"

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
#include "qtconceptmapcommandunselectedge.h"
#include "qtconceptmapcommandunselectnode.h"

ribi::cmap::CommandUnselect::CommandUnselect(
  QtConceptMap& qtconceptmap,
  QGraphicsItem& item
)  : Command(qtconceptmap),
     m_cmd{nullptr}
{
  if (QtEdge* const qtedge = dynamic_cast<QtEdge*>(&item))
  {
    m_cmd = new CommandUnselectEdge(qtconceptmap, qtedge, this);
  }
  else if (QtNode* const qtnode = dynamic_cast<QtNode*>(&item))
  {
    if (QtEdge* const qtedge2 = FindQtEdge(qtnode, GetQtConceptMap()))
    {
      m_cmd = new CommandUnselectEdge(qtconceptmap, qtedge2, this);
    }
    else
    {
      m_cmd = new CommandUnselectNode(qtconceptmap, qtnode, this);
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
    if (QtEdge * const qtedge = dynamic_cast<QtEdge*>(item))
    {
      if (GetText(*qtedge) == t)
      {
        return new CommandUnselect(qtconceptmap, *qtedge);
      }
    }
    if (QtNode * const qtnode = dynamic_cast<QtNode*>(item))
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
