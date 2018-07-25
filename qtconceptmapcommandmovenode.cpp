#include "qtconceptmapcommandmovenode.h"

#include <cassert>
//#include <boost/graph/isomorphism.hpp>
#include <boost/algorithm/string/trim.hpp>


#include <gsl/gsl_assert>
//#include <QApplication>
#include "container.h"
#include "conceptmap.h"
#include "conceptmaphelper.h"
#include "conceptmapnode.h"
#include "qtconceptmap.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmaphelper.h"

ribi::cmap::CommandMoveNode::CommandMoveNode(
  QtConceptMap& qtconceptmap,
  QtNode * const qtnode,
  const double dx,
  const double dy
)
  : Command(qtconceptmap),
    m_qtnode{qtnode},
    m_dx{dx},
    m_dy{dy}
{
  if (!m_qtnode)
  {
    throw std::invalid_argument("Cannot move nullptr QtNode");
  }
  if (!IsMovable(*m_qtnode))
  {
    throw std::invalid_argument("Cannot move unmovable QtNode");
  }
  if (IsQtNodeOnEdge(m_qtnode))
  {
    //Use CommandMoveEdge to move an edge
    throw std::invalid_argument("Cannot move QtNode on QtEdge");
  }

  //QCommands have a text
  {
    std::stringstream msg;
    msg << "Move node with dx="
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
  //"move_node(my_text, 10, 20)"
  boost::algorithm::trim(s);
  const std::string str_begin = "move_node(";
  if (s.substr(0, str_begin.size()) != str_begin) return nullptr;
  if (s.back() != ')') return nullptr;
  //"my text, 10, 20"
  const std::string t = s.substr(str_begin.size(), s.size() - str_begin.size() - 1);
  assert(t[0] != '(');
  assert(t.back() != ')');
  // "my text", "10", "20"
  std::vector<std::string> v = Container().SeperateString(t, ',');
  for (std::string& u: v) boost::algorithm::trim(u);
  try
  {
    const std::string text = v.at(0);
    const double dx = std::stod(v.at(1));
    const double dy = std::stod(v.at(2));
    QtNode * first_qtnode = FindFirstQtNode(
      qtconceptmap,
      [&qtconceptmap, text](const QtNode * const qtnode)
      {
        return GetText(*qtnode) == text
          && !IsOnEdge(*qtnode)
          && IsMovable(*qtnode)
        ;
      }
    );
    return new CommandMoveNode(qtconceptmap, first_qtnode, dx, dy);
  }
  catch (std::exception&) {} //OK
  return nullptr;
}

void ribi::cmap::CommandMoveNode::Redo()
{
  assert(m_qtnode);
  assert(!IsOnEdge(*m_qtnode));
  assert(IsMovable(*m_qtnode));
  MoveQtNode(*m_qtnode, m_dx, m_dy, GetQtConceptMap());  
}

void ribi::cmap::CommandMoveNode::Undo()
{
  assert(m_qtnode);
  assert(!IsOnEdge(*m_qtnode));
  assert(IsMovable(*m_qtnode));
  MoveQtNode(*m_qtnode, -m_dx, -m_dy, GetQtConceptMap());
}
