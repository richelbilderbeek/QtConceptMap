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
  const std::string& name,
  const double dx,
  const double dy
)
  : Command(qtconceptmap),
    m_dx{dx},
    m_dy{dy},
    m_moved_qtnode{nullptr},
    m_name{name}
{
  //QCommands have a text
  {
    std::stringstream msg;
    msg << "MoveNode node or edge with name '"
      << m_name << "'";
    this->setText(msg.str().c_str());
  }
}

std::function<bool(const ribi::cmap::Node&)> ribi::cmap::NodeHasText(
  const std::string& text) noexcept
{
  return [text](const Node& node) { return GetText(node) == text; };
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
    return new CommandMoveNode(qtconceptmap, text, dx, dy);
  }
  catch (std::exception&) {} //OK
  return nullptr;
}

void ribi::cmap::CommandMoveNode::redo()
{
  CheckInvariantQtEdgesAndEdgesHaveSameCoordinats(GetQtConceptMap());
  CheckInvariantQtNodesAndNodesHaveSameCoordinats(GetQtConceptMap());

  m_moved_qtnode = FindFirstQtNode(GetQtConceptMap(),
    [name = m_name, &q = GetQtConceptMap()](QtNode * const qtnode)
    {
      return name == GetText(*qtnode) && !IsOnEdge(*qtnode, q)
        && qtnode->flags() & QGraphicsItem::ItemIsMovable
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
    msg << "Cannot find movable QtNode with name '"
      << this->GetName() << "'";
    throw std::runtime_error(msg.str());
  }

  assert(!(0 ^ 0));
  assert((0 ^ 1));
  assert((1 ^ 0));
  assert(!(1 ^ 1));
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
