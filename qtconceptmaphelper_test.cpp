#include "qtconceptmaphelper_test.h"
#include "qtconceptmaphelper.h"
#include "conceptmapfactory.h"

#include "qtconceptmap.h"

void ribi::cmap::qtconceptmaphelper_test::get_qt_center_node()
{
  //No conecpt map, thus no center node
  {
    QtConceptMap m;
    const auto qtnode = GetQtCenterNode(m.GetScene());
    QVERIFY(!qtnode);
  }
  //Concept map of one center node
  {
    QtConceptMap m;
    m.SetConceptMap(ConceptMapFactory().Get1());
    const auto qtnode = GetQtCenterNode(m.GetScene());
    QVERIFY(qtnode);
  }
}

void ribi::cmap::qtconceptmaphelper_test::mess_up()
{
  QtConceptMap m;
  m.SetConceptMap(ConceptMapFactory().Get11());
  m.show();
  QTest::qWait(1000);
  MoveQtEdgesAndQtNodesRandomly(m);
  m.show();
  QTest::qWait(1000);
}


