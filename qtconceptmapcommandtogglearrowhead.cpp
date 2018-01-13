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
#include "qtconceptmap.h"
#include "qtconceptmaptoolsitem.h"
#include "qtconceptmaphelper.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmapqtnode.h"

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
  QtEdge * const m_qtedge = ExtractTheOneSelectedQtEdge(GetScene(*this));
  m_qtedge->SetHasHeadArrow(!HasHeadArrow(*m_qtedge));
}

void ribi::cmap::CommandToggleArrowHead::Undo()
{
  Redo();
}
