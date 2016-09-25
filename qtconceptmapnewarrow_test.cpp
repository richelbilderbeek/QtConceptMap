#include "qtconceptmapnewarrow_test.h"

#include "qtconceptmapnewarrow.h"
#include "qtconceptmap.h"
#include "qtconceptmaphelper.h"

void ribi::cmap::qtconceptmapnewarrow_test::all_tests()
{
  QtConceptMap m;
  m.show();
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  const auto qtnodes = GetQtNodes(m.GetScene());
  assert(qtnodes.size() == 1);
  QtNewArrow q;
  q.Start(qtnodes[0]);
  m.show();

}
