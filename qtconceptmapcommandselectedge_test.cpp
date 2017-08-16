#include "qtconceptmapcommandselectedge_test.h"

#include "qtconceptmapcommandselectedge.h"
#include "qtconceptmap.h"
#include "conceptmapfactory.h"

void ribi::cmap::QtConceptMapCommandSelectEdgeTest::SelectAbsentItemFails() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  try
  {
    q.DoCommand(new CommandSelectEdge(q, "absent"));
    QVERIFY(!"Should not get here");
  }
  catch (std::exception&)
  {
    QVERIFY("OK");
  }
}

void ribi::cmap::QtConceptMapCommandSelectEdgeTest::SelectQtCenterNodeByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  q.DoCommand(new CommandSelectEdge(q, "center"));
  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 1);
}

void ribi::cmap::QtConceptMapCommandSelectEdgeTest::SelectQtEdgeByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdgeNoCenter());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  q.DoCommand(new CommandSelectEdge(q, "second"));
  QVERIFY(CountSelectedQtEdges(q) == 1);
  QVERIFY(CountSelectedQtNodes(q) == 0);
}

void ribi::cmap::QtConceptMapCommandSelectEdgeTest::SelectQtEdgeConnectedToCenterByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  //q.showFullScreen();
  assert(CountSelectedQtNodes(q) == 0);
  q.DoCommand(new CommandSelectEdge(q, "first"));
  assert(GetQtEdges(q).size() == 1);
  assert(CountSelectedQtEdges(q) == 1);
  QVERIFY(CountSelectedQtEdges(q) == 1);
  QVERIFY(CountSelectedQtNodes(q) == 0);
}

void ribi::cmap::QtConceptMapCommandSelectEdgeTest::SelectQtNodeByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  q.DoCommand(new CommandSelectEdge(q, "one"));
  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 1);
}

void ribi::cmap::QtConceptMapCommandSelectEdgeTest::Parse() const noexcept
{
  QtConceptMap q;
  const auto c = ParseCommandSelectEdge(q, "select(my text)");
  QVERIFY(c != nullptr);
  QVERIFY(c->GetName() == "my text");
}

void ribi::cmap::QtConceptMapCommandSelectEdgeTest::ParseNonsenseFails() const noexcept
{
  QtConceptMap q;
  QVERIFY(ParseCommandSelectEdge(q, "nonsense") == nullptr);
}
