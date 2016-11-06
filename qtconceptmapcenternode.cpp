
/*














*/



#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "qtconceptmapcenternode.h"

#include <stdexcept>

#include "qtconceptmapbrushfactory.h"
#include "conceptmapnode.h"
#pragma GCC diagnostic pop

ribi::cmap::QtCenterNode::QtCenterNode(const Node& node)
  : QtNode(node)
{
  if (!IsCenterNode(node))
  {
    throw std::invalid_argument(
      "QtCenterNode: must be constructed from a CenterNode"
     );
  }
  this->setFlags(QGraphicsItem::ItemIsFocusable);
}
