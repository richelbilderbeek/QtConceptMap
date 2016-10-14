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
  assert(!qtnodes.empty());
  const auto qtnode = qtnodes.back();
  m.GetQtNewArrow().Start(qtnode);
  QTest::mouseMove(&m); //Puts mouse in center of widget
  m.show();
  QTest::qWait(1000);
}
