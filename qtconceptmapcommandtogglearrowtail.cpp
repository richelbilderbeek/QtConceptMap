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
#include <boost/algorithm/string/trim.hpp>
#include "find_first_custom_edge.h"
#include <QGraphicsScene>
#include "qtconceptmap.h"
#include "qtconceptmaptoolsitem.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmapqtnode.h"
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
  boost::algorithm::trim(s);
  if (s == "toggle_tail()" || s == "toggle_arrow_tail()")
  {
    return new CommandToggleTail(qtconceptmap);
  }
  return nullptr;
}

void ribi::cmap::CommandToggleArrowTail::Redo()
{
  QtEdge * const qtedge = ExtractTheOneSelectedQtEdge(GetScene(*this));

  qtedge->SetHasTailArrow(
    !HasTailArrow(*qtedge)
  );
}

void ribi::cmap::CommandToggleArrowTail::Undo()
{
  Redo();
}
