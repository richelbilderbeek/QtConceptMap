#ifndef QTCONCEPTMAPCENTERNODE_H
#define QTCONCEPTMAPCENTERNODE_H

#ifdef KEEP_CENTER_QTNODE

#include "qtconceptmapqtnode.h"
#include "conceptmapnode.h"

namespace ribi {
namespace cmap {

///The center node
struct QtCenterNode : public QtNode
{
  ///Node cannot be const, as it contains a Concept the user might want to edit
  ///(that is, when a sub-concept map is created from a concept map and the
  ///focal node needs to be rated)
  ///concept_item is the display Strategy
  explicit QtCenterNode(const Node& node);
};

} //~namespace cmap
} //~namespace ribi

#endif // KEEP_CENTER_QTNODE

#endif // QTCONCEPTMAPCENTERNODE_H
