#include "qtconceptmapcommandunselectall.h"

#include <cassert>
#include <boost/algorithm/string/trim.hpp>
#include <gsl/gsl_assert>
#include "count_vertices_with_selectedness.h"
#include "container.h"
#include "find_first_custom_vertex_with_my_vertex.h"
#include "find_first_custom_edge_with_my_edge.h"
#include "set_edge_selectedness.h"
#include "set_vertex_selectedness.h"
#include "conceptmap.h"
#include "conceptmaphelper.h"
#include "conceptmapnode.h"
#include "qtconceptmap.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmaphelper.h"
#include "qtconceptmapcommandunselect.h"
#include <QGraphicsScene>

ribi::cmap::CommandUnselectAll::CommandUnselectAll(
  QtConceptMap& qtconceptmap,
  QUndoCommand *parent
)  : Command(qtconceptmap, parent),
     m_cmd{new QUndoCommand(this)}
{
  //Unselect the QtNodes
  for (QGraphicsItem * const item: GetQtConceptMap().GetScene().selectedItems())
  {
    assert(item);
    assert(item->isSelected());

    //Do not add a QtNode that is on a QtEdge, only keep that QtEdge
    if (qgraphicsitem_cast<QtNode*>(item)
      || qgraphicsitem_cast<QtNode*>(item)
    )
    {
      try
      {
        assert(qgraphicsitem_cast<QtNode*>(item));
        new CommandUnselect(GetQtConceptMap(), *item, m_cmd);
      }
      catch (std::exception&) {} //OK
    }
  }

  if (!m_cmd->childCount())
  {
    throw std::invalid_argument("Cannot unselect if none selected");
  }
  //QCommands have a text
  {
    std::stringstream msg;
    msg << "Unselect all (" << m_cmd->childCount() << " items)";
    this->setText(msg.str().c_str());
  }
}

ribi::cmap::CommandUnselectAll::~CommandUnselectAll() noexcept
{

}

ribi::cmap::CommandUnselectAll * ribi::cmap::ParseCommandUnselectAll(
  QtConceptMap& qtconceptmap, std::string s)
{
  //"unselect_all()"
  boost::algorithm::trim(s);
  const std::string str_begin = "unselect_all(";
  if (s.substr(0, str_begin.size()) != str_begin) return nullptr;
  if (s.back() != ')') return nullptr;
  //""
  const std::string t = s.substr(str_begin.size(), s.size() - str_begin.size() - 1);
  //assert(t[0] != '(');
  //assert(t.back() != ')');
  if (!t.empty()) return nullptr;
  return new CommandUnselectAll(qtconceptmap);
}

void ribi::cmap::CommandUnselectAll::Redo()
{
  m_cmd->redo();
}

void ribi::cmap::CommandUnselectAll::Undo()
{
  m_cmd->undo();
}
