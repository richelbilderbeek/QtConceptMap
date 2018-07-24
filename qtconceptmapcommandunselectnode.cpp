#include "qtconceptmapcommandunselectnode.h"

#include <cassert>
//#include <boost/graph/isomorphism.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <gsl/gsl_assert>
//#include <QApplication>
#include "count_vertices_with_selectedness.h"
#include "container.h"
#include "conceptmap.h"
#include "conceptmaphelper.h"
#include "conceptmapnode.h"
#include "qtconceptmap.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmaphelper.h"

ribi::cmap::CommandUnselectNode::CommandUnselectNode(
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
    throw std::invalid_argument("Cannot unselect nullptr QtNode");
  }
  if (IsQtNodeOnEdge(qtnode, GetQtConceptMap()))
  {
    throw std::invalid_argument(
      "Cannot unselect QtNode on QtEdge, "
      "use CommandUnselectEdge instead");
  }
  if (!m_qtnode->isSelected())
  {
    throw std::invalid_argument("Cannot unselect QtNode that is already unselected");
  }
  //QCommands have a text
  {
    std::stringstream msg;
    msg << "Unselect node";
    this->setText(msg.str().c_str());
  }
}

ribi::cmap::CommandUnselectNode * ribi::cmap::ParseCommandUnselectNode(
  QtConceptMap& qtconceptmap, std::string s)
{
  //"unselect(my text)"
  boost::algorithm::trim(s);
  const std::string str_begin = "unselect_node(";
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
  return new CommandUnselectNode(qtconceptmap, first_qtnode);
}

void ribi::cmap::CommandUnselectNode::Redo()
{
  #ifndef NDEBUG
  const int n_selected_qtnodes_before = CountSelectedQtNodes(GetQtConceptMap());
  #endif

  m_prev_qttoolitem_buddy = GetQtToolItemBuddy(GetQtConceptMap());

  assert(m_qtnode);
  Unselect(GetQtConceptMap(), *m_qtnode);

  #ifndef NDEBUG
  const int n_selected_qtnodes_after = CountSelectedQtNodes(GetQtConceptMap());
  Ensures(n_selected_qtnodes_after < n_selected_qtnodes_before);
  #endif
}

void ribi::cmap::CommandUnselectNode::Undo()
{
  SetQtToolItemBuddy(GetQtConceptMap(), m_prev_qttoolitem_buddy);
  SetSelectedness(true, *m_qtnode);
}
