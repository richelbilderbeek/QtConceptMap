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
    QtEdge * const first_qtedge = FindFirstQtEdge(
      q,
      [](QtEdge * const qtedge) { return GetText(*qtedge) == "absent"; }
    );
    q.DoCommand(new CommandSelectEdge(q, first_qtedge));
    QVERIFY(!"Should not get here");
  }
  catch (std::exception&)
  {
    QVERIFY("OK");
  }
}

void ribi::cmap::QtConceptMapCommandSelectEdgeTest::SelectQtEdgeByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdgeNoCenter());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);

  QtEdge * const first_qtedge = FindFirstQtEdge(
    q,
    [](QtEdge * const qtedge) { return GetText(*qtedge) == "second"; }
  );
  q.DoCommand(new CommandSelectEdge(q, first_qtedge));

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

  QtEdge * const first_qtedge = FindFirstQtEdge(
    q,
    [](QtEdge * const qtedge) { return GetText(*qtedge) == "first"; }
  );
  q.DoCommand(new CommandSelectEdge(q, first_qtedge));

  QVERIFY(CountSelectedQtEdges(q) == 1);
  QVERIFY(CountSelectedQtNodes(q) == 0);
}

void ribi::cmap::QtConceptMapCommandSelectEdgeTest::Parse() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  const auto c = ParseCommandSelectEdge(q, "select_edge(first)");
  assert(c);
  QVERIFY(c != nullptr);
  QVERIFY(GetText(*c->GetQtEdge()) == "first");
}

void ribi::cmap::QtConceptMapCommandSelectEdgeTest::ParseNonsenseFails() const noexcept
{
  QtConceptMap q;
  QVERIFY(ParseCommandSelectEdge(q, "nonsense") == nullptr);
}