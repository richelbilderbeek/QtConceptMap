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
#include "trace.h"
#include "count_vertices_with_selectedness.h"
#include "add_edge_between_selected_vertices.h"
#include <boost/graph/isomorphism.hpp>
#include "find_first_custom_edge_with_my_edge.h"
#include <QGraphicsScene>
#include "qtconceptmap.h"
#include "qtconceptmaptoolsitem.h"
#include "qtconceptmaphelper.h"

ribi::cmap::CommandToggleArrowTail::CommandToggleArrowTail(
  ConceptMap& conceptmap,
  QGraphicsScene * const scene
) : m_conceptmap(conceptmap),
    m_edge_before{ExtractTheOneSelectedEdge(conceptmap,*scene)},
    m_scene{scene},
    m_qtedge{ExtractTheOneSelectedQtEdge(*scene)}
{
  this->setText("Toggle arrow tail");
}

void ribi::cmap::CommandToggleArrowTail::redo()
{
  //Find the edge with the desired ID
  const auto ed = ::find_first_custom_edge_with_my_edge(
    m_edge_before, m_conceptmap,
    [](const Edge& lhs, const Edge& rhs) { return lhs.GetId() == rhs.GetId(); }
  );
  auto current_edge = get_my_custom_edge(ed, m_conceptmap);

  const auto has_arrow_old = current_edge.HasTailArrow();
  const auto has_arrow_new = !has_arrow_old;

  //Add an arrow and put it back in the concept map
  current_edge.SetTailArrow(has_arrow_new);
  ::set_my_custom_edge(current_edge, ed, m_conceptmap);

  //Put the current arrow tail in the QtEdge
  m_qtedge->SetHasTailArrow(has_arrow_new);

  assert(m_qtedge->HasTailArrow() == current_edge.HasTailArrow());
  assert(m_qtedge->HasTailArrow() == m_qtedge->GetEdge().HasTailArrow());
}

void ribi::cmap::CommandToggleArrowTail::undo()
{
  redo();
}
