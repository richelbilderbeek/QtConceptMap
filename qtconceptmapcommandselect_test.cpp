#include "qtconceptmapcommandselect_test.h"

#include "qtconceptmapcommandselect.h"
#include "qtconceptmap.h"
#include "conceptmapfactory.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmapqtnode.h"

void ribi::cmap::QtCommandSelectTest::SelectQtCenterNodeByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  q.DoCommand(new CommandSelect(q, *FindFirstQtNode(q, QtNodeHasName("center"))));
  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 1);
}

void ribi::cmap::QtCommandSelectTest::SelectQtEdgeByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdgeNoCenter());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  q.DoCommand(new CommandSelect(q, *FindFirstQtEdge(q, QtEdgeHasName("second"))));
  QVERIFY(CountSelectedQtEdges(q) == 1);
  QVERIFY(CountSelectedQtNodes(q) == 0);
}

void ribi::cmap::QtCommandSelectTest::SelectQtEdgeConnectedToCenterByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  //q.showFullScreen();
  assert(CountSelectedQtNodes(q) == 0);
  q.DoCommand(new CommandSelect(q, *FindFirstQtEdge(q, QtEdgeHasName("first"))));
  assert(GetQtEdges(q).size() == 1);
  assert(CountSelectedQtEdges(q) == 1);
  QVERIFY(CountSelectedQtEdges(q) == 1);
  QVERIFY(CountSelectedQtNodes(q) == 0);
}

void ribi::cmap::QtCommandSelectTest::SelectQtNodeByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  q.DoCommand(new CommandSelect(q, *FindFirstQtNode(q, QtNodeHasName("one"))));
  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 1);
}

void ribi::cmap::QtCommandSelectTest::ParseCenterNode() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  const auto c = ParseCommandSelect(q, "select(center)");
  QVERIFY(c != nullptr);
}

void ribi::cmap::QtCommandSelectTest::ParseEdge() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  const auto c = ParseCommandSelect(q, "select(first)");
  QVERIFY(c != nullptr);
}

void ribi::cmap::QtCommandSelectTest::ParseNode() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  const auto c = ParseCommandSelect(q, "select(one)");
  QVERIFY(c != nullptr);
}

void ribi::cmap::QtCommandSelectTest::ParseNonsenseFails() const noexcept
{
  QtConceptMap q;
  QVERIFY(ParseCommandSelect(q, "nonsense") == nullptr);
}
