#include "qtconceptmapcommandselectnode.h"

#include <cassert>
#include <stdexcept>
#include <boost/algorithm/string/trim.hpp>
#include <gsl/gsl_assert>
#include <QDebug>
#include "conceptmaphelper.h"
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
  if (IsOnEdge(*qtnode))
  {
    qCritical() << "Cannot select QtNode on QtEdge, use CommandSelectEdge instead";
    throw std::invalid_argument(
      "Cannot select QtNode on QtEdge, use CommandSelectEdge instead"
    );
  }
  if (m_qtnode->isSelected())
  {
    qCritical() << "Cannot select QtNode that is already selected";
    throw std::invalid_argument("Cannot select QtNode that is already selected");
  }
  //QCommands have a text
  {
    std::stringstream msg;
    msg << "Select node with text '" << GetText(*m_qtnode) << "'";
    this->setText(msg.str().c_str());
  }
}

ribi::cmap::CommandSelectNode::~CommandSelectNode()
{

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
        && !IsQtNodeOnEdge(qtnode);
    }
  );
  return new CommandSelectNode(qtconceptmap, first_qtnode);
}

void ribi::cmap::CommandSelectNode::Redo()
{
  m_prev_qttoolitem_buddy = GetQtToolItemBuddy(GetQtConceptMap());

  Select(GetQtConceptMap(), *m_qtnode);
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
    SetQtToolItemBuddy(GetQtConceptMap(), no_qtnode);
  }

  SetSelectedness(false, *m_qtnode);
}
