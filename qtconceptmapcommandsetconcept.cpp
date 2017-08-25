#include "qtconceptmapcommandsetconcept.h"

#include <cassert>
#include <boost/algorithm/string/trim.hpp>
#include <gsl/gsl_assert>
#include "qtconceptmap.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmaphelper.h"

ribi::cmap::CommandSetConcept::CommandSetConcept(
  QtConceptMap& qtconceptmap,
  const Concept& concept
)
  : Command(qtconceptmap),
    m_concept{concept}
{
  if (!GetQtToolItemBuddy(GetQtConceptMap()))
  {
    throw std::invalid_argument("QtToolItem must have a buddy to have its Concept set");
  }
  if (IsQtCenterNode(GetQtToolItemBuddy(GetQtConceptMap())))
  {
    throw std::invalid_argument("Cannot set Concept of center node");
  }
  //QCommands have a text
  {
    std::stringstream msg;
    msg << "Set concept";
    this->setText(msg.str().c_str());
  }
}

ribi::cmap::CommandSetConcept * ribi::cmap::ParseCommandSetConcept(
  QtConceptMap& qtconceptmap, std::string s)
{
  //"set_concept(<my xml>)"
  boost::algorithm::trim(s);
  const std::string str_begin = "set_concept(";
  if (s.substr(0, str_begin.size()) != str_begin) return nullptr;
  if (s.back() != ')') return nullptr;
  //"<my xml>"
  const std::string xml = s.substr(str_begin.size(), s.size() - str_begin.size() - 1);
  assert(xml[0] != '(');
  assert(xml.back() != ')');
  const Concept concept = XmlToConcept(xml);
  return new CommandSetConcept(qtconceptmap, concept);
}

///Class T may be either a QtNode or a QtEdge
template <class T>
void RedoImpl(
  ribi::cmap::CommandSetConcept& cmd,
  T * const t
)
{
  static_assert(
    std::is_same<T, ribi::cmap::QtNode>() || std::is_same<T, ribi::cmap::QtEdge>(),
    "T is either QtEdge or QtNode");

  cmd.SetPrevConcept(GetConcept(*t));

  SetConcept(*t, cmd.GetConcept());

  assert(GetConcept(*t) == cmd.GetConcept());

  if (HasExamples(*t))
  {
    SetQtExamplesBuddy(cmd.GetQtConceptMap(), t);
  }
  else
  {
    T * const no_qtnode{nullptr};
    SetQtExamplesBuddy(cmd.GetQtConceptMap(), no_qtnode);
  }
}

void ribi::cmap::CommandSetConcept::Redo()
{
  Expects(GetQtToolItemBuddy(GetQtConceptMap()));
  Expects(!IsQtCenterNode(GetQtToolItemBuddy(GetQtConceptMap())));

  QtNode * const qtnode = GetQtToolItemBuddy(GetQtConceptMap());
  assert(qtnode);
  QtEdge * const qtedge = FindQtEdge(qtnode, GetQtConceptMap());

  if (qtedge)
  {
    RedoImpl(*this, qtedge);
  }
  else
  {
    RedoImpl(*this, qtnode);
  }
}

///Class T may be either a QtNode or a QtEdge
template <class T>
void UndoImpl(
  ribi::cmap::CommandSetConcept& cmd,
  T * const t
)
{
  static_assert(
    std::is_same<T, ribi::cmap::QtNode>() || std::is_same<T, ribi::cmap::QtEdge>(),
    "T is either QtEdge or QtNode");

  SetConcept(*t, cmd.GetPrevConcept());

  assert(GetConcept(*t) == cmd.GetPrevConcept());

  if (HasExamples(*t))
  {
    SetQtExamplesBuddy(cmd.GetQtConceptMap(), t);
  }
  else
  {
    T * const no_qtnode{nullptr};
    SetQtExamplesBuddy(cmd.GetQtConceptMap(), no_qtnode);
  }
}


void ribi::cmap::CommandSetConcept::Undo()
{
  QtNode * const qtnode = GetQtToolItemBuddy(GetQtConceptMap());
  assert(qtnode);
  QtEdge * const qtedge = FindQtEdge(qtnode, GetQtConceptMap());

  if (qtedge)
  {
    UndoImpl(*this, qtedge);
  }
  else
  {
    UndoImpl(*this, qtnode);
  }
}
