#include "qtconceptmapcommandunselectall.h"

#ifdef FIRST_FINISH_COMMAND_SELECT_ALL

#include <cassert>
#include <QGraphicsScene>
//#include <boost/graph/isomorphism.hpp>
//#include <boost/algorithm/string/trim.hpp>
//#include <boost/algorithm/string/trim_all.hpp>
//#include <boost/graph/adjacency_list.hpp>
//#include <boost/lexical_cast.hpp>

//#include <gsl/gsl_assert>
//#include <QApplication>
//#include "count_vertices_with_selectedness.h"
//#include "container.h"
//#include "find_first_custom_vertex_with_my_vertex.h"
//#include "find_first_custom_edge_with_my_edge.h"
//#include "set_edge_selectedness.h"
//#include "set_vertex_selectedness.h"
//#include "conceptmap.h"
//#include "conceptmaphelper.h"
//#include "conceptmapnode.h"
//#include "qtconceptmap.h"
//#include "qtconceptmapqtnode.h"
//#include "qtconceptmaphelper.h"

ribi::cmap::CommandUnselectAll::CommandUnselectAll(
  QtConceptMap& qtconceptmap
)
  : Command(qtconceptmap)
{
  //QCommands have a text
  {
    std::stringstream msg;
    msg << "Unselect all";
    this->setText(msg.str().c_str());
  }

  for (QGraphicsItem * const item: GetScene(*this).selectedItems())
  {
    QtNode * const qtnode = dynamic_cast<QtNode*>(item);
    if (IsQtNodeOnEdge(qtnode, q))
    {
      QtEdge * const qtedge = FindQtEdge(qtnode, q.GetScene());
      new CommandUnselectEdge(q, qtedge, this);
    }
    else if (qtnode)
    {
      new CommandUnselectNode(q, qtnode, this);
    }
    else if (QtEdge * const qtedge = dynamic_cast<QtEdge*>(item))
    {
      assert(!"Will this ever be triggered?");
      new CommandUnselectEdge(q, qtedge, this);
    }
  }
}

ribi::cmap::CommandUnselectAll * ribi::cmap::ParseCommandUnselectAll(
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
  return new CommandUnselectAll(qtconceptmap, t);
}

void ribi::cmap::CommandUnselectAll::redo()
{
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
  qApp->processEvents();
}

void ribi::cmap::CommandUnselectAll::undo()
{
  if (m_unselected_qtedge)
  {
    SetSelectedness(true, *m_unselected_qtedge, GetQtConceptMap());
  }
  if (m_unselected_qtnode)
  {
    SetSelectedness(true, *m_unselected_qtnode, GetQtConceptMap());
  }
}

#endif // FIRST_FINISH_COMMAND_SELECT_ALL
