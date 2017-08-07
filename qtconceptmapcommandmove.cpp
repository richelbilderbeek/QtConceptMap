#include "qtconceptmapcommandmove.h"

#include <cassert>
#include <boost/graph/isomorphism.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/trim_all.hpp>
#include <boost/lexical_cast.hpp>
#include <gsl/gsl_assert>
#include <QApplication>
#include "container.h"
#include "conceptmap.h"
#include "conceptmaphelper.h"
#include "conceptmapnode.h"
#include "qtconceptmap.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmaphelper.h"

ribi::cmap::CommandMove::CommandMove(
  QtConceptMap& qtconceptmap,
  const std::string& name,
  const double dx,
  const double dy
)
  : Command(qtconceptmap),
    m_dx{dx},
    m_dy{dy},
    m_moved_qtedge{nullptr},
    m_moved_qtnode{nullptr},
    m_name{name}
{
  //QCommands have a text
  {
    std::stringstream msg;
    msg << "Move node or edge with name '"
      << m_name << "'";
    this->setText(msg.str().c_str());
  }
}

ribi::cmap::CommandMove * ribi::cmap::parse_command_move(
  QtConceptMap& qtconceptmap, std::string s)
{
  //"move(my_text, 10, 20)"
  boost::algorithm::trim(s);
  const std::string str_begin = "move(";
  if (s.substr(0, str_begin.size()) != str_begin) return nullptr;
  if (s.back() != ')') return nullptr;
  //"my text, 10, 20"
  const std::string t = s.substr(str_begin.size(), s.size() - str_begin.size() - 1);
  assert(t[0] != '(');
  assert(t.back() != ')');
  // "my text, 10", "20"
  std::vector<std::string> v = Container().SeperateString(t, ',');
  for (std::string& u: v) boost::algorithm::trim(u);
  try
  {
    const std::string text = v.at(0);
    const double dx = std::stod(v.at(1));
    const double dy = std::stod(v.at(2));
    return new CommandMove(qtconceptmap, text, dx, dy);
  }
  catch (std::exception&) {} //OK
  return nullptr;
}

void ribi::cmap::CommandMove::redo()
{
  CheckInvariantQtNodesAndNodesHaveSameCoordinats(GetQtConceptMap());

  m_moved_qtnode = FindFirstQtNode(GetQtConceptMap().GetScene(),
    [name = m_name](QtNode * const qtnode)
    {
      return name == GetText(*qtnode);
    }
  );
  if (m_moved_qtnode)
  {
    m_moved_qtedge = nullptr;
    Move(*m_moved_qtnode, m_dx, m_dy);
  }
  else
  {
    assert(!m_moved_qtnode);
    m_moved_qtedge = FindFirstQtEdge(GetQtConceptMap().GetScene(),
      [name = m_name](QtEdge * const qtedge)
      {
        return name == GetText(*qtedge);
      }
    );
    if (m_moved_qtedge)
    {
      Move(*m_moved_qtedge, m_dx, m_dy);
    }
  }


  qApp->processEvents();
  CheckInvariantQtNodesAndNodesHaveSameCoordinats(GetQtConceptMap());
  CheckInvariants(GetQtConceptMap());
}

void ribi::cmap::CommandMove::undo()
{
  CheckInvariantQtNodesAndNodesHaveSameCoordinats(GetQtConceptMap());

  if (m_moved_qtedge)
  {
    m_moved_qtedge->moveBy(-m_dx, -m_dy);
  }
  if (m_moved_qtnode)
  {
    m_moved_qtnode->moveBy(-m_dx, -m_dy);
  }
  CheckInvariantQtNodesAndNodesHaveSameCoordinats(GetQtConceptMap());
  CheckInvariants(GetQtConceptMap());
}
