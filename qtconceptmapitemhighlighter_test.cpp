#include "qtconceptmapitemhighlighter_test.h"
#include "qtconceptmapitemhighlighter.h"
#include "conceptmapfactory.h"
#include "qtconceptmap.h"
#include "qtconceptmaphelper.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmapqtnode.h"

void ribi::cmap::QtItemHighlighterTest::ItemCanBeCenterQtNode() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetLonelyCenterNode());
  q.show();
  QtItemHighlighter& h = q.GetQtHighlighter();
  h.SetItem(GetFirstQtNode(q));
  QTest::qWait(100);
  QVERIFY(h.GetItem() == GetFirstQtNode(q));
}

void ribi::cmap::QtItemHighlighterTest::ItemCanBeNormalQtNode() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetLonelyNode());
  q.show();
  QtItemHighlighter& h = q.GetQtHighlighter();
  h.SetItem(GetFirstQtNode(q));
  QTest::qWait(100);
  QVERIFY(h.GetItem() == GetFirstQtNode(q));
}

void ribi::cmap::QtItemHighlighterTest::ItemCannotBeQtNodeOnQtEdge() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  q.show();
  QtItemHighlighter& h = q.GetQtHighlighter();
  try
  {
    h.SetItem(GetFirstQtEdge(q)->GetQtNode());
    assert(!"Should not get here");
  }
  catch (const std::exception&)
  {
    QVERIFY("Should get here");
  }
}

void ribi::cmap::QtItemHighlighterTest::NoItemAtConstruction() const noexcept
{
  const QtItemHighlighter h;
  QVERIFY(!h.GetItem());
}
