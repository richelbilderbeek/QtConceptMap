#include "qtconceptmapcommandunselectnode.h"

#include <cassert>
#include <boost/algorithm/string/trim.hpp>
#include <gsl/gsl_assert>

#include <QDebug>

#include "conceptmap.h"
#include "conceptmaphelper.h"
#include "conceptmapnode.h"
#include "container.h"
#include "count_vertices_with_selectedness.h"
#include "qtconceptmap.h"
#include "qtconceptmaphelper.h"
#include "qtconceptmapqtnode.h"

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
  if (IsQtNodeOnEdge(qtnode))
  {
    throw std::invalid_argument(
      "Cannot unselect QtNode on QtEdge, use CommandUnselectEdge instead"
    );
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

ribi::cmap::CommandUnselectNode::~CommandUnselectNode() noexcept
{

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
        && !IsQtNodeOnEdge(qtnode);
    }
  );
  return new CommandUnselectNode(qtconceptmap, first_qtnode);
}

void ribi::cmap::CommandUnselectNode::Redo()
{
  m_prev_qttoolitem_buddy = GetQtToolItemBuddy(GetQtConceptMap());
  assert(m_qtnode);
  Unselect(GetQtConceptMap(), *m_qtnode);
}

void ribi::cmap::CommandUnselectNode::Undo()
{
  SetQtToolItemBuddy(GetQtConceptMap(), m_prev_qttoolitem_buddy);
  SetSelectedness(true, *m_qtnode);
}
