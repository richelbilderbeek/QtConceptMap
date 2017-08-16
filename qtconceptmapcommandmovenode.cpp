#include "qtconceptmapcommandmovenode.h"

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

ribi::cmap::CommandMoveNode::CommandMoveNode(
  QtConceptMap& qtconceptmap,
  const std::function<bool(const QtNode&)>& predicate,
  const double dx,
  const double dy
)
  : Command(qtconceptmap),
    m_moved_qtnode{nullptr},
    m_predicate{predicate},
    m_dx{dx},
    m_dy{dy}
{
  //QCommands have a text
  {
    std::stringstream msg;
    msg << "Move node by predicate with dx="
      << m_dx << " and dy=" << m_dy;
    this->setText(msg.str().c_str());
  }
}

std::function<bool(const ribi::cmap::QtNode&)> ribi::cmap::QtNodeHasText(
  const std::string& text) noexcept
{
  return [text](const QtNode& qtnode) { return GetText(qtnode) == text; };
}

ribi::cmap::CommandMoveNode * ribi::cmap::ParseCommandMoveNode(
  QtConceptMap& qtconceptmap, std::string s)
{
  //"movenode(my_text, 10, 20)"
  boost::algorithm::trim(s);
  const std::string str_begin = "move_node(";
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
    return new CommandMoveNode(qtconceptmap, QtNodeHasText(text), dx, dy);
  }
  catch (std::exception&) {} //OK
  return nullptr;
}

void ribi::cmap::CommandMoveNode::redo()
{
  CheckInvariantQtEdgesAndEdgesHaveSameCoordinats(GetQtConceptMap());
  CheckInvariantQtNodesAndNodesHaveSameCoordinats(GetQtConceptMap());

  m_moved_qtnode = FindFirstQtNode(GetQtConceptMap(),
    [predicate = m_predicate, &q = GetQtConceptMap()](QtNode * const qtnode)
    {
      return predicate(*qtnode)
        && !IsOnEdge(*qtnode, q)
        && IsMovable(*qtnode)
      ;
    }
  );
  if (m_moved_qtnode)
  {
    assert(!IsOnEdge(*m_moved_qtnode, GetQtConceptMap()));
    MoveQtNode(*m_moved_qtnode, m_dx, m_dy, GetQtConceptMap());
    CheckInvariantQtEdgesAndEdgesHaveSameCoordinats(GetQtConceptMap());
  }
  else
  {
    assert(!m_moved_qtnode);
    std::stringstream msg;
    msg << "Cannot find movable QtNode";
    throw std::runtime_error(msg.str());
  }

  assert(m_moved_qtnode);

  CheckInvariantQtEdgesAndEdgesHaveSameCoordinats(GetQtConceptMap());
  CheckInvariantQtNodesAndNodesHaveSameCoordinats(GetQtConceptMap());

  qApp->processEvents();

  CheckInvariantQtEdgesAndEdgesHaveSameCoordinats(GetQtConceptMap());
  CheckInvariantQtNodesAndNodesHaveSameCoordinats(GetQtConceptMap());
  CheckInvariants(GetQtConceptMap());
}

void ribi::cmap::CommandMoveNode::undo()
{
  CheckInvariantQtNodesAndNodesHaveSameCoordinats(GetQtConceptMap());

  if (m_moved_qtnode)
  {
    MoveQtNode(*m_moved_qtnode, -m_dx, -m_dy, GetQtConceptMap());
  }
  CheckInvariantQtNodesAndNodesHaveSameCoordinats(GetQtConceptMap());
  CheckInvariants(GetQtConceptMap());
}
