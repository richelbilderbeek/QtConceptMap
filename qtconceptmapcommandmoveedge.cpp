#include "qtconceptmapcommandmoveedge.h"

#include <cassert>
#include <boost/algorithm/string/trim.hpp>
#include <gsl/gsl_assert>
//#include <QApplication>
#include "container.h"
#include "conceptmaphelper.h"
#include "conceptmapnode.h"
#include "qtconceptmap.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmaphelper.h"

ribi::cmap::CommandMoveEdge::CommandMoveEdge(
  QtConceptMap& qtconceptmap,
  QtEdge * const qtedge,
  const double dx,
  const double dy
)
  : Command(qtconceptmap),
    m_qtedge{qtedge},
    m_dx{dx},
    m_dy{dy}
{
  if (!m_qtedge)
  {
    throw std::invalid_argument("Cannot move nullptr QtEdge");
  }
  if (!IsMovable(*m_qtedge))
  {
    throw std::invalid_argument("Cannot move unmovable QtEdge");
  }
  //QCommands have a text
  {
    std::stringstream msg;
    msg << "Move edge by predicate";
    this->setText(msg.str().c_str());
  }
}

ribi::cmap::CommandMoveEdge * ribi::cmap::ParseCommandMoveEdge(
  QtConceptMap& qtconceptmap, std::string s)
{
  //"move_edge(my_text, 10, 20)"
  boost::algorithm::trim(s);
  const std::string str_begin = "move_edge(";
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
    QtEdge * const first_edge = FindFirstQtEdge(
      qtconceptmap,
      [text](QtEdge * const qtedge)
      {
        return GetText(*qtedge) == text;
      }
    );
    return new CommandMoveEdge(qtconceptmap, first_edge, dx, dy);
  }
  catch (std::exception&) {} //OK
  return nullptr;
}

std::function<bool(const ribi::cmap::QtEdge&)> ribi::cmap::QtEdgeHasText(
  const std::string& text) noexcept
{
  return [text](const QtEdge& qtedge) { return GetText(qtedge) == text; };
}

void ribi::cmap::CommandMoveEdge::Redo()
{
  assert(m_qtedge);
  assert(IsInScene(*m_qtedge, GetScene(*this)));

  MoveQtEdge(*m_qtedge, m_dx, m_dy, GetQtConceptMap());

  assert(IsInScene(*m_qtedge, GetScene(*this)));
}

void ribi::cmap::CommandMoveEdge::Undo()
{
  assert(m_qtedge);
  MoveQtEdge(*m_qtedge, -m_dx, -m_dy, GetQtConceptMap());
}
