#include "qtconceptmapcommandtogglearrowhead.h"

#include <cassert>
#include <sstream>

#include "conceptmap.h"

#include "conceptmapedgefactory.h"
#include "conceptmapnode.h"
#include "get_my_custom_edge.h"
#include "set_my_custom_edge.h"
#include "qtconceptmap.h"
#include "qtconceptmapqtnode.h"
#include "count_vertices_with_selectedness.h"
#include "add_edge_between_selected_vertices.h"
#include "find_first_custom_edge.h"
#include <boost/algorithm/string/trim.hpp>
#include "find_first_custom_edge_with_my_edge.h"
#include <QGraphicsScene>
#include "qtconceptmap.h"
#include "qtconceptmaptoolsitem.h"
#include "qtconceptmaphelper.h"

ribi::cmap::CommandToggleArrowHead::CommandToggleArrowHead(
  QtConceptMap& qtconceptmap
) : Command(qtconceptmap)
{
  {
    std::stringstream msg;
    msg << "Toggle arrow head";
    this->setText(msg.str().c_str());
  }
}

ribi::cmap::CommandToggleArrowHead * ribi::cmap::ParseCommandToggleArrowHead(
  QtConceptMap& qtconceptmap, std::string s)
{
  boost::algorithm::trim(s);
  if (s == "toggle_head()" || s == "toggle_arrow_head()")
  {
    return new CommandToggleHead(qtconceptmap);
  }
  return nullptr;
}

void ribi::cmap::CommandToggleArrowHead::Redo()
{
  const Edge m_edge_before = ExtractTheOneSelectedEdge(GetConceptMap(*this), GetScene(*this));
  QtEdge * const m_qtedge = ExtractTheOneSelectedQtEdge(GetScene(*this));

  //Find the edge with the desired ID
  const auto ed = ::find_first_custom_edge(
    [id = m_edge_before.GetId()](const Edge& edge) { return edge.GetId() == id; },
    GetConceptMap(*this)
  );
  auto current_edge = get_my_custom_edge(ed, GetConceptMap(*this));

  const auto has_arrow_old = current_edge.HasHeadArrow();
  const auto has_arrow_new = !has_arrow_old;

  //Add an arrow and put it back in the concept map
  current_edge.SetHeadArrow(has_arrow_new);
  ::set_my_custom_edge(current_edge, ed, GetConceptMap(*this));

  //Put the current arrow head in the QtEdge
  m_qtedge->SetHasHeadArrow(has_arrow_new);

  assert(HasHeadArrow(*m_qtedge) == current_edge.HasHeadArrow());
  assert(HasHeadArrow(*m_qtedge) == m_qtedge->GetEdge().HasHeadArrow());
}

void ribi::cmap::CommandToggleArrowHead::Undo()
{
  Redo();
}
