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
#include <boost/graph/isomorphism.hpp>
#include "find_first_custom_edge_with_my_edge.h"
#include <QGraphicsScene>
#include "qtconceptmap.h"
#include "qtconceptmaptoolsitem.h"
#include "qtconceptmaphelper.h"

ribi::cmap::CommandToggleArrowHead::CommandToggleArrowHead(
  QtConceptMap& qtconceptmap
) : Command(qtconceptmap),
    m_conceptmap(qtconceptmap.GetConceptMap()),
    m_edge_before{ExtractTheOneSelectedEdge(qtconceptmap.GetConceptMap(),qtconceptmap.GetScene())},
    m_scene{qtconceptmap.GetScene()},
    m_qtedge{ExtractTheOneSelectedQtEdge(qtconceptmap.GetScene())}
{
  this->setText("Toggle arrow head");
}

void ribi::cmap::CommandToggleArrowHead::redo()
{
  //Find the edge with the desired ID
  const auto ed = ::find_first_custom_edge(
    [id = m_edge_before.GetId()](const Edge& edge) { return edge.GetId() == id; },
    m_conceptmap
  );
  auto current_edge = get_my_custom_edge(ed, m_conceptmap);

  const auto has_arrow_old = current_edge.HasHeadArrow();
  const auto has_arrow_new = !has_arrow_old;

  //Add an arrow and put it back in the concept map
  current_edge.SetHeadArrow(has_arrow_new);
  ::set_my_custom_edge(current_edge, ed, m_conceptmap);

  //Put the current arrow head in the QtEdge
  m_qtedge->SetHasHeadArrow(has_arrow_new);

  assert(HasHeadArrow(*m_qtedge) == current_edge.HasHeadArrow());
  assert(HasHeadArrow(*m_qtedge) == m_qtedge->GetEdge().HasHeadArrow());
}

void ribi::cmap::CommandToggleArrowHead::undo()
{
  redo();
}
