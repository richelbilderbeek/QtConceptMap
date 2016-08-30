#include "qtconceptmapqtnodefactory_test.h"
#include "qtconceptmapqtnodefactory.h"

void ribi::cmap::qtconceptmapqtnodefactory_test::all_tests()
{
  using namespace ribi::cmap;
  const QtNodeFactory f;
  const int n = f.GetNumberOfTests();
  for (int i=0; i!=n; ++i)
  {
    const auto qtnode = f.GetTest(i);
    QVERIFY(qtnode.get()); //Need .get for QVERIFY
  }
}
