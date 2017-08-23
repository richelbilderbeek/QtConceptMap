#include "qtconceptmapcommandselectnode.h"

#include <cassert>
#include <stdexcept>
#include <boost/algorithm/string/trim.hpp>
#include <gsl/gsl_assert>
#include "qtconceptmapqtedge.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmap.h"

ribi::cmap::CommandSelectNode::CommandSelectNode(
  QtConceptMap& qtconceptmap,
  QtNode * const qtnode,
  QUndoCommand * const parent
)
  : Command(qtconceptmap, parent),
    m_prev_qttoolitem_buddy{nullptr},
    m_qtnode{qtnode}
{
  if (!m_qtnode)
  {
    throw std::invalid_argument("Cannot select nullptr QtNode");
  }
  if (IsQtNodeOnEdge(qtnode, GetQtConceptMap()))
  {
    throw std::invalid_argument(
      "Cannot select QtNode on QtEdge, "
      "use CommandSelectEdge instead");
  }
  if (m_qtnode->isSelected())
  {
    throw std::invalid_argument("Cannot select QtNode that is already selected");
  }
  //QCommands have a text
  {
    std::stringstream msg;
    msg << "Select node";
    this->setText(msg.str().c_str());
  }
}

ribi::cmap::CommandSelectNode * ribi::cmap::ParseCommandSelectNode(
  QtConceptMap& qtconceptmap, std::string s)
{
  //"select(my text)"
  boost::algorithm::trim(s);
  const std::string str_begin = "select_node(";
  if (s.substr(0, str_begin.size()) != str_begin) return nullptr;
  if (s.back() != ')') return nullptr;
  //"my text"
  const std::string t = s.substr(str_begin.size(), s.size() - str_begin.size() - 1);
  assert(t[0] != '(');
  assert(t.back() != ')');
  QtNode * const first_qtnode = FindFirstQtNode(
    qtconceptmap,
    [&qtconceptmap, t](QtNode * const qtnode)
    {
      return GetText(*qtnode) == t
        && !IsQtNodeOnEdge(qtnode, qtconceptmap);
    }
  );
  return new CommandSelectNode(qtconceptmap, first_qtnode);
}

void ribi::cmap::CommandSelectNode::Redo()
{
  #ifndef NDEBUG
  const int n_selected_qtnodes_before = CountSelectedQtNodes(GetQtConceptMap());
  #endif

  m_prev_qttoolitem_buddy = GetQtToolItemBuddy(GetQtConceptMap());

  Select(GetQtConceptMap(), *m_qtnode);

  #ifndef NDEBUG
  const int n_selected_qtnodes_after = CountSelectedQtNodes(GetQtConceptMap());
  Ensures(n_selected_qtnodes_after > n_selected_qtnodes_before);
  #endif
}

void ribi::cmap::CommandSelectNode::Undo()
{
  if (m_prev_qttoolitem_buddy)
  {
    Select(GetQtConceptMap(), *m_prev_qttoolitem_buddy);
  }
  else
  {
    QtNode * const no_qtnode{nullptr};
    SetQtExamplesBuddy(GetQtConceptMap(), no_qtnode);
    SetQtToolItemBuddy(GetQtConceptMap(), no_qtnode);
  }

  SetSelectedness(false, *m_qtnode, GetQtConceptMap());
}
