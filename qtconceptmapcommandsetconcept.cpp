#include "qtconceptmapcommandsetconcept.h"

#include <cassert>
#include <boost/graph/isomorphism.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/trim_all.hpp>
#include <boost/lexical_cast.hpp>
#include <gsl/gsl_assert>
#include <QApplication>
#include "count_vertices_with_selectedness.h"
#include "container.h"
#include "conceptmap.h"
#include "conceptmaphelper.h"
#include "conceptmapnode.h"
#include "qtconceptmap.h"
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

void ribi::cmap::CommandSetConcept::redo()
{
  CheckInvariants(GetQtConceptMap());

  Expects(GetQtToolItemBuddy(GetQtConceptMap()));
  Expects(!IsQtCenterNode(GetQtToolItemBuddy(GetQtConceptMap())));

  QtNode * const qtnode = GetQtToolItemBuddy(GetQtConceptMap());
  assert(qtnode);

  m_prev_concept = GetConcept(*qtnode);

  SetConcept(*qtnode, m_concept);

  assert(GetConcept(*qtnode) == m_concept);

  if (HasExamples(*qtnode))
  {
    SetQtExamplesBuddy(GetQtConceptMap(), qtnode);
  }
  else
  {
    const QtNode * const no_qtnode{nullptr};
    SetQtExamplesBuddy(GetQtConceptMap(), no_qtnode);
  }

  CheckInvariants(GetQtConceptMap());
}

void ribi::cmap::CommandSetConcept::undo()
{
  CheckInvariants(GetQtConceptMap());

  QtNode * const qtnode = GetQtToolItemBuddy(GetQtConceptMap());
  assert(qtnode);

  SetConcept(*qtnode, m_prev_concept);

  assert(GetConcept(*qtnode) == m_prev_concept);

  if (HasExamples(*qtnode))
  {
    SetQtExamplesBuddy(GetQtConceptMap(), qtnode);
  }
  else
  {
    const QtNode * const no_qtnode{nullptr};
    SetQtExamplesBuddy(GetQtConceptMap(), no_qtnode);
  }

  CheckInvariants(GetQtConceptMap());
}
