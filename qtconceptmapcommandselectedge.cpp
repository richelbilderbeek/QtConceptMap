#include "qtconceptmapcommandselectedge.h"

#include <cassert>
#include <boost/algorithm/string/trim.hpp>
#include <gsl/gsl_assert>
#include "qtconceptmap.h"
#include "qtconceptmapqtedge.h"

ribi::cmap::CommandSelectEdge::CommandSelectEdge(
  QtConceptMap& qtconceptmap,
  QtEdge * const qtedge,
  QUndoCommand * const parent
)
  : Command(qtconceptmap, parent),
    m_qtedge{qtedge}
{

  if (!m_qtedge)
  {
    throw std::invalid_argument("Cannot select a nullptr QtEdge");
  }
  //Add mildness: we need to select. If one of the two is already selected, just select both
  if (m_qtedge->isSelected() && m_qtedge->GetQtNode()->isSelected())
  {
    throw std::invalid_argument("Cannot select a QtEdge that is already selected");
  }

  //QCommands have a text
  {
    std::stringstream msg;
    msg << "Select edge with text '" << GetText(*m_qtedge) << "'";
    this->setText(msg.str().c_str());
  }
}

ribi::cmap::CommandSelectEdge::~CommandSelectEdge() noexcept
{

}

ribi::cmap::CommandSelectEdge * ribi::cmap::ParseCommandSelectEdge(
  QtConceptMap& qtconceptmap, std::string s)
{
  //"select(my text)"
  boost::algorithm::trim(s);
  const std::string str_begin = "select_edge(";
  if (s.substr(0, str_begin.size()) != str_begin) return nullptr;
  if (s.back() != ')') return nullptr;
  //"my text"
  const std::string t = s.substr(str_begin.size(), s.size() - str_begin.size() - 1);
  assert(t[0] != '(');
  assert(t.back() != ')');
  QtEdge * const first_qtedge = FindFirstQtEdge(
    qtconceptmap,
    [t](const QtEdge* qtedge) { return GetText(*qtedge) == t; }
  );
  return new CommandSelectEdge(qtconceptmap, first_qtedge);
}

void ribi::cmap::CommandSelectEdge::Redo()
{
  m_prev_qttoolitem_buddy = GetQtToolItemBuddy(GetQtConceptMap());

  assert(m_qtedge);

  Select(GetQtConceptMap(), *m_qtedge);
}

void ribi::cmap::CommandSelectEdge::Undo()
{
  SetQtToolItemBuddy(GetQtConceptMap(), m_prev_qttoolitem_buddy);
  SetSelectedness(false, *m_qtedge);
}
