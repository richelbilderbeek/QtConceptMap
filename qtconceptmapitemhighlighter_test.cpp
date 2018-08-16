#include "qtconceptmapitemhighlighter_test.h"
#include "qtconceptmapitemhighlighter.h"
#include "conceptmapfactory.h"
#include "qtconceptmap.h"
#include "qtconceptmaphelper.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmapqtnode.h"

void ribi::cmap::QtConceptMapItemHighlighterTest::AllTests()
{
  QtConceptMap m;
  m.SetConceptMap(ConceptMapFactory().Get2());
  m.show();
  QtItemHighlighter& h = m.GetQtHighlighter();
  const std::vector<QtNode *> qtnodes = GetQtNodes(m.GetScene());
  assert(!qtnodes.empty());
  const auto qtnode = qtnodes.at(1);
  assert(qtnode);
  assert(!IsQtCenterNode(qtnode));
  h.SetItem(qtnode);
  m.show();
  QTest::qWait(1000);
  h.Stop();
  QTest::qWait(1000);
  m.show();
}

void ribi::cmap::QtConceptMapItemHighlighterTest::Construction()
{
  const QtItemHighlighter h;
  QVERIFY(!h.GetItem());
}
