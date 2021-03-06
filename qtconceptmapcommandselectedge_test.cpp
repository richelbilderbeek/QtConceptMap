#include "qtconceptmapcommandselectedge_test.h"

#include "qtconceptmapcommandselectedge.h"
#include "qtconceptmapcommandselectnode.h"
#include "qtconceptmap.h"
#include "conceptmapfactory.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmapqtnode.h"

void ribi::cmap::QtCommandSelectEdgeTest::SelectAbsentItemFails() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  try
  {
    QtEdge * const first_qtedge = FindFirstQtEdge(q, QtEdgeHasName("absent"));
    q.DoCommand(new CommandSelectEdge(q, first_qtedge));
    QVERIFY(!"Should not get here");
  }
  catch (std::exception&)
  {
    QVERIFY("OK");
  }
}

void ribi::cmap::QtCommandSelectEdgeTest::SelectQtEdgeByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdgeNoCenter());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);

  QtEdge * const first_qtedge = FindFirstQtEdge(q, QtEdgeHasName("second"));
  const QtNode * const first_qtnode = first_qtedge->GetQtNode();
  q.DoCommand(new CommandSelectEdge(q, first_qtedge));

  QVERIFY(CountSelectedQtEdges(q) == 1);
  QVERIFY(CountSelectedQtNodes(q) == 0);
  QVERIFY(GetQtToolItemBuddy(q) == nullptr);
}

void ribi::cmap::QtCommandSelectEdgeTest::SelectQtEdgeByNameAndUndo() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdgeNoCenter());

  QtNode * const first_qtnode = FindFirstQtNode(q, QtNodeHasName("one"));
  q.DoCommand(new CommandSelectNode(q, first_qtnode));

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 1);
  assert(GetQtToolItemBuddy(q) == first_qtnode);

  QtEdge * const first_qtedge = FindFirstQtEdge(q, QtEdgeHasName("second"));
  q.DoCommand(new CommandSelectEdge(q, first_qtedge));

  assert(CountSelectedQtEdges(q) == 1);
  assert(CountSelectedQtNodes(q) == 1);
  assert(GetQtToolItemBuddy(q) == nullptr);

  q.Undo();

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 1);
  assert(GetQtToolItemBuddy(q) == first_qtnode);
}


void ribi::cmap::QtCommandSelectEdgeTest::SelectQtEdgeConnectedToCenterByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);

  QtEdge * const first_qtedge = FindFirstQtEdge(q, QtEdgeHasName("first"));
  q.DoCommand(new CommandSelectEdge(q, first_qtedge));

  QVERIFY(CountSelectedQtEdges(q) == 1);
  QVERIFY(CountSelectedQtNodes(q) == 0);
}

void ribi::cmap::QtCommandSelectEdgeTest
  ::SelectSelectedQtEdgeFails() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdgeNoCenter());

  QtEdge * const first_qtedge = FindFirstQtEdge(q, QtEdgeHasName("second"));
  q.DoCommand(new CommandSelectEdge(q, first_qtedge));

  assert(CountSelectedQtEdges(q) == 1);
  assert(CountSelectedQtNodes(q) == 0);

  try
  {
    q.DoCommand(new CommandSelectEdge(q, first_qtedge));
    QVERIFY(!"Should not get here"); //!OCLINT accepted idiom
  }
  catch (std::invalid_argument&)
  {
    QVERIFY("OK"); //!OCLINT accepted idiom
  }
}

void ribi::cmap::QtCommandSelectEdgeTest::SelectTwoQtEdgesByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetThreeNodeTwoEdgeNoCenter());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);

  QtEdge * const first_qtedge = FindFirstQtEdge(q, QtEdgeHasName("first"));
  const QtNode * const first_qtnode = first_qtedge->GetQtNode();
  q.DoCommand(new CommandSelectEdge(q, first_qtedge));

  QVERIFY(CountSelectedQtEdges(q) == 1);
  QVERIFY(CountSelectedQtNodes(q) == 0);
  QVERIFY(HasExamples(*first_qtedge));
  QVERIFY(GetQtToolItemBuddy(q) ==  nullptr);

  QtEdge * const second_qtedge = FindFirstQtEdge(q, QtEdgeHasName("second"));
  const QtNode * const second_qtnode = second_qtedge->GetQtNode();
  q.DoCommand(new CommandSelectEdge(q, second_qtedge));

  QVERIFY(CountSelectedQtEdges(q) == 2);
  QVERIFY(CountSelectedQtNodes(q) == 0);
  QVERIFY(HasExamples(*second_qtedge));
  QVERIFY(GetQtToolItemBuddy(q) == nullptr);
}

void ribi::cmap::QtCommandSelectEdgeTest::Parse() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  const auto c = ParseCommandSelectEdge(q, "select_edge(first)");
  assert(c);
  QVERIFY(c != nullptr);
  QVERIFY(GetText(*c->GetQtEdge()) == "first");
}

void ribi::cmap::QtCommandSelectEdgeTest::ParseNonsenseFails() const noexcept
{
  QtConceptMap q;
  QVERIFY(ParseCommandSelectEdge(q, "nonsense") == nullptr);
}
