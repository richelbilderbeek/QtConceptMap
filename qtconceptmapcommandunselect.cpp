#include "qtconceptmapcommandunselect.h"

#include <cassert>
#include <boost/graph/isomorphism.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/trim_all.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/lexical_cast.hpp>

#include <gsl/gsl_assert>
#include <QApplication>
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
#include "qtconceptmapqtnode.h"
#include "qtconceptmaphelper.h"

ribi::cmap::CommandUnselect::CommandUnselect(
  QtConceptMap& qtconceptmap,
  const std::string& name
)
  : Command(qtconceptmap),
    m_unselected_qtedge{nullptr},
    m_unselected_qtnode{nullptr},
    m_name{name}
{
  //QCommands have a text
  {
    std::stringstream msg;
    msg << "Select node or edge with name '"
      << m_name << "'";
    this->setText(msg.str().c_str());
  }
}

ribi::cmap::CommandUnselect * ribi::cmap::parse_command_unselect(
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
  return new CommandUnselect(qtconceptmap, t);
}

void ribi::cmap::CommandUnselect::redo()
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

void ribi::cmap::CommandUnselect::undo()
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
