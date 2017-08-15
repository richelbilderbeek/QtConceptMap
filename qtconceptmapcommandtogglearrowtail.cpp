#include "qtconceptmapcommandtogglearrowtail.h"

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
#include <boost/graph/isomorphism.hpp>
#include "find_first_custom_edge.h"
#include <QGraphicsScene>
#include "qtconceptmap.h"
#include "qtconceptmaptoolsitem.h"
#include "qtconceptmaphelper.h"

ribi::cmap::CommandToggleArrowTail::CommandToggleArrowTail(
  QtConceptMap& qtconceptmap
) : Command(qtconceptmap)
{
  this->setText("Toggle arrow tail");
}

ribi::cmap::CommandToggleArrowTail * ribi::cmap::ParseCommandToggleArrowTail(
  QtConceptMap& qtconceptmap, std::string s)
{
  if (s == "toggle_tail()" || s == "toggle_arrow_tail()")
  {
    return new CommandToggleTail(qtconceptmap);
  }
  return nullptr;
}

void ribi::cmap::CommandToggleArrowTail::redo()
{
  const Edge m_edge_before = ExtractTheOneSelectedEdge(GetConceptMap(*this), GetScene(*this));
  QtEdge * const m_qtedge = ExtractTheOneSelectedQtEdge(GetScene(*this));

  //Find the edge with the desired ID
  const auto ed = ::find_first_custom_edge(
    [id = m_edge_before.GetId()](const Edge& edge) { return edge.GetId() == id; },
    GetConceptMap(*this)
  );
  auto current_edge = get_my_custom_edge(ed, GetConceptMap(*this));

  const auto has_arrow_old = current_edge.HasTailArrow();
  const auto has_arrow_new = !has_arrow_old;

  //Add an arrow and put it back in the concept map
  current_edge.SetTailArrow(has_arrow_new);
  ::set_my_custom_edge(current_edge, ed, GetConceptMap(*this));

  //Put the current arrow tail in the QtEdge
  m_qtedge->SetHasTailArrow(has_arrow_new);

  assert(HasTailArrow(*m_qtedge) == current_edge.HasTailArrow());
  assert(HasTailArrow(*m_qtedge) == m_qtedge->GetEdge().HasTailArrow());
}

void ribi::cmap::CommandToggleArrowTail::undo()
{
  redo();
}
