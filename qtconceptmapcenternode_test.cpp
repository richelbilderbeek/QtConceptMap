#include "qtconceptmapcenternode_test.h"
#include "qtconceptmapcenternode.h"

#include "conceptmapnodefactory.h"
#include "conceptmapcenternodefactory.h"

void ribi::cmap::qtconceptmapcenternode_test::all_tests() const noexcept
{
  //Must be constructed from a center node
  QtCenterNode qtnode(ribi::cmap::CenterNodeFactory().GetNasty0());
  QVERIFY(qtnode.GetNode().IsCenterNode());


  //Fails if not constructed from a center node
  try
  {
    QtCenterNode(ribi::cmap::NodeFactory().GetNasty0());
    QVERIFY(!"Should not get here");
  }
  catch (std::invalid_argument& e)
  {
    QVERIFY("Should not get here");
  }
}
