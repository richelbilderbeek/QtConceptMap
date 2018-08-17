#include "qtconceptmaphelper_test.h"
#include "qtconceptmaphelper.h"
#include "conceptmapfactory.h"

#include "qtconceptmap.h"

void ribi::cmap::QtConceptMapHelperTest::ExtractTheOneSelectedQtEdge() const noexcept
{
  try
  {
    ::ribi::cmap::ExtractTheOneSelectedQtEdge(QGraphicsScene());
    assert(!"Should not get here");
  }
  catch(const std::invalid_argument&)
  {
    QVERIFY("Should get here");
  }
}

void ribi::cmap::QtConceptMapHelperTest::GetQtCenterNode() const noexcept
{
  //No conecpt map, thus no center node
  {
    QtConceptMap m;
    const auto qtnode = ::ribi::cmap::GetQtCenterNode(m);
    QVERIFY(!qtnode);
  }
  //Concept map of one center node
  {
    QtConceptMap m;
    m.SetConceptMap(ConceptMapFactory().Get1());
    const auto qtnode = ::ribi::cmap::GetQtCenterNode(m);
    QVERIFY(qtnode);
  }
}

void ribi::cmap::QtConceptMapHelperTest::MessUp() const noexcept
{
  QtConceptMap m;
  m.SetConceptMap(ConceptMapFactory().Get11());
  m.show();
  MoveQtEdgesAndQtNodesRandomly(m);
}


