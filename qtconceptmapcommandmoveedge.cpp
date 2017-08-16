#include "qtconceptmapcommandmoveedge.h"

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

ribi::cmap::CommandMoveEdge::CommandMoveEdge(
  QtConceptMap& qtconceptmap,
  const std::function<bool(const QtEdge&)> predicate,
  const double dx,
  const double dy
)
  : Command(qtconceptmap),
    m_moved_qtedge{nullptr},
    m_predicate{predicate},
    m_dx{dx},
    m_dy{dy}
{
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
    return new CommandMoveEdge(qtconceptmap, QtEdgeHasText(text), dx, dy);
  }
  catch (std::exception&) {} //OK
  return nullptr;
}

std::function<bool(const ribi::cmap::QtEdge&)> ribi::cmap::QtEdgeHasText(
  const std::string& text) noexcept
{
  return [text](const QtEdge& qtedge) { return GetText(qtedge) == text; };
}

void ribi::cmap::CommandMoveEdge::redo()
{
  CheckInvariantQtEdgesAndEdgesHaveSameCoordinats(GetQtConceptMap());
  CheckInvariantQtNodesAndNodesHaveSameCoordinats(GetQtConceptMap());

  m_moved_qtedge = FindFirstQtEdge(GetScene(*this),
    [predicate = m_predicate](QtEdge * const qtedge)
    {
      return predicate(*qtedge);
    }
  );
  if (m_moved_qtedge)
  {
    assert(
      CountQtEdges(GetQtConceptMap()) != 1 ||
      HasSameData(m_moved_qtedge->GetEdge(), GetFirstEdge(GetQtConceptMap().GetConceptMap()))
    );
    assert(IsInScene(*m_moved_qtedge, GetScene(*this)));

    MoveQtEdge(*m_moved_qtedge, m_dx, m_dy, GetQtConceptMap());

    assert(IsInScene(*m_moved_qtedge, GetScene(*this)));


    assert(
      CountQtEdges(GetQtConceptMap()) != 1 ||
      HasSameData(m_moved_qtedge->GetEdge(), GetFirstEdge(GetConceptMap(*this)))
    );

    assert(
      CountQtEdges(GetQtConceptMap()) != 1 ||
      HasSameData(m_moved_qtedge->GetEdge(), GetFirstEdge(GetQtConceptMap().GetConceptMap()))
    );

    CheckInvariantQtEdgesAndEdgesHaveSameCoordinats(GetQtConceptMap());
  }
  else
  {
    std::stringstream msg;
    msg << "Cannot find movable QtEdge";
    throw std::runtime_error(msg.str());
  }

  assert(m_moved_qtedge);

  CheckInvariantQtEdgesAndEdgesHaveSameCoordinats(GetQtConceptMap());
  CheckInvariantQtNodesAndNodesHaveSameCoordinats(GetQtConceptMap());

  qApp->processEvents();

  CheckInvariantQtEdgesAndEdgesHaveSameCoordinats(GetQtConceptMap());
  CheckInvariantQtNodesAndNodesHaveSameCoordinats(GetQtConceptMap());
  CheckInvariants(GetQtConceptMap());
}

void ribi::cmap::CommandMoveEdge::undo()
{
  CheckInvariantQtNodesAndNodesHaveSameCoordinats(GetQtConceptMap());

  if (m_moved_qtedge)
  {
    MoveQtEdge(*m_moved_qtedge, -m_dx, -m_dy, GetQtConceptMap());
  }
  CheckInvariantQtNodesAndNodesHaveSameCoordinats(GetQtConceptMap());
  CheckInvariants(GetQtConceptMap());
}
