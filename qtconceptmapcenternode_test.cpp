#include "qtconceptmapcenternode_test.h"

#include <cassert>

#include "conceptmapnodefactory.h"
#include "conceptmapcenternodefactory.h"
#include "qtconceptmapcenternode.h"

void ribi::cmap::qtconceptmapcenternode_test::all_tests() const noexcept
{
  //Must be constructed from a center node
  QtCenterNode qtnode(ribi::cmap::CenterNodeFactory().GetNasty0());
  QVERIFY(qtnode.GetNode().IsCenterNode());

  //Fails if not constructed from a center node
  try
  {
    const QtCenterNode c(ribi::cmap::NodeFactory().GetNasty0());
    assert(!c.GetText().empty()); //Will not get here
    QVERIFY(!"Should not get here");
  }
  catch (std::invalid_argument& e)
  {
    QVERIFY("Should get here");
  }
}
