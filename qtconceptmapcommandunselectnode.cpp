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
  qCritical() << __func__ << ": " << __LINE__ << ": " << "CommandUnselectNode c'tor start";
  if (!m_qtnode)
  {
    qCritical() << __func__ << ": " << __LINE__ << ": " << "CommandUnselectNode c'tor qtnode nullptr";
    throw std::invalid_argument("Cannot unselect nullptr QtNode");
  }
  if (IsQtNodeOnEdge(qtnode))
  {
    qCritical() << __func__ << ": " << __LINE__ << ": " << "CommandUnselectNode c'tor qtnode is on edge";
    throw std::invalid_argument(
      "Cannot unselect QtNode on QtEdge, use CommandUnselectEdge instead"
    );
  }
  if (!m_qtnode->isSelected())
  {
    qCritical() << __func__ << ": " << __LINE__ << ": " << "CommandUnselectNode c'tor qtnode not seleected";
    throw std::invalid_argument("Cannot unselect QtNode that is already unselected");
  }
  qCritical() << __func__ << ": " << __LINE__ << ": " << "CommandUnselectNode c'tor before text";
  //QCommands have a text
  {
    std::stringstream msg;
    msg << "Unselect node";
    this->setText(msg.str().c_str());
  }
  qCritical() << __func__ << ": " << __LINE__ << ": " << "CommandUnselectNode c'tor end";
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
  qCritical() << __func__ << ": " << __LINE__ << ": " << "CommandUnselectNode::Redo start";
  m_prev_qttoolitem_buddy = GetQtToolItemBuddy(GetQtConceptMap());
  qCritical() << __func__ << ": " << __LINE__ << ": " << "CommandUnselectNode::Redo mid";
  assert(m_qtnode);
  Unselect(GetQtConceptMap(), *m_qtnode);
  qCritical() << __func__ << ": " << __LINE__ << ": " << "CommandUnselectNode::Redo end";
}

void ribi::cmap::CommandUnselectNode::Undo()
{
  qCritical() << __func__ << ": " << __LINE__ << ": " << "CommandUnselectNode::Undo start";
  SetQtToolItemBuddy(GetQtConceptMap(), m_prev_qttoolitem_buddy);
  qCritical() << __func__ << ": " << __LINE__ << ": " << "CommandUnselectNode::Undo mid";
  SetSelectedness(true, *m_qtnode);
  qCritical() << __func__ << ": " << __LINE__ << ": " << "CommandUnselectNode::Undo end";
}
