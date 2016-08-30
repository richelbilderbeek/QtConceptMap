#include "qtconceptmapcommandaddselectedrandom.h"

#include <cassert>
#include <sstream>

#include "conceptmap.h"
#include "conceptmaphelper.h"
#include "conceptmapnode.h"
#include "trace.h"

ribi::cmap::CommandAddSelectedRandom::CommandAddSelectedRandom(ConceptMap& conceptmap)
  :
    m_conceptmap(conceptmap),
    m_before{conceptmap},
    m_after{RandomlyAddSelectedNode(conceptmap)}
{
  assert(!"Am I used?");
}

void ribi::cmap::CommandAddSelectedRandom::redo()
{
  m_conceptmap = m_after;
}

void ribi::cmap::CommandAddSelectedRandom::undo()
{
  m_conceptmap = m_before;
}

ribi::cmap::ConceptMap ribi::cmap::RandomlyAddSelectedNode(ConceptMap g)
{
  std::vector<VertexDescriptor> v;
  const auto vip = vertices(g);

  std::copy_if(
    vip.first,
    vip.second,
    std::back_inserter(v),
    [g](const VertexDescriptor& vd)
    {
      const auto is_selected_map
        = get(boost::vertex_is_selected, g);
      return get(is_selected_map, vd);
    }
  );
  if (v.empty())
  {
    std::stringstream s;
    s << __func__ << ": need non-selected node to selected";
    throw std::logic_error(s.str());
  }


  const auto is_selected_map
    = get(boost::vertex_is_selected, g);
  put(
    is_selected_map,
    v[ std::rand() % v.size() ],
    true
  );
  return g;
}
