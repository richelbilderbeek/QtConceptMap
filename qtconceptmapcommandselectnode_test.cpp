#include "qtconceptmapcommandselectnode_test.h"

#include "qtconceptmapcommandselectnode.h"
#include "qtconceptmap.h"
#include "conceptmapfactory.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmapqtnode.h"

void ribi::cmap::QtCommandSelectNodeTest::SelectAbsentItemFails() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  try
  {
    QtNode * const first_qtnode = FindFirstQtNode(q, QtNodeHasName("absent"));
    q.DoCommand(new CommandSelectNode(q, first_qtnode));
    QVERIFY(!"Should not get here");
  }
  catch (std::exception&)
  {
    QVERIFY("OK");
  }
}

void ribi::cmap::QtCommandSelectNodeTest::SelectQtCenterNodeByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  QtNode * const first_qtnode = FindFirstQtNode(q, QtNodeHasName("center"));
  q.DoCommand(new CommandSelectNode(q, first_qtnode));

  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 1);
}

void ribi::cmap::QtCommandSelectNodeTest
  ::SelectOneOfTwoQtNodesByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);

  QtNode * const first_qtnode = FindFirstQtNode(q, QtNodeHasName("one"));
  q.DoCommand(new CommandSelectNode(q, first_qtnode));

  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 1);
}

void ribi::cmap::QtCommandSelectNodeTest
  ::SelectOneOfTwoQtNodesByNameAndUndo() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);

  const auto tool_buddy_before = GetQtToolItemBuddy(q);

  QtNode * const first_qtnode = FindFirstQtNode(q, QtNodeHasName("one"));
  q.DoCommand(new CommandSelectNode(q, first_qtnode));

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 1);
  assert(HasExamples(*first_qtnode));
  assert(tool_buddy_before != GetQtToolItemBuddy(q));

  q.Undo();

  const auto tool_buddy_after = GetQtToolItemBuddy(q);

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  QVERIFY(tool_buddy_before == tool_buddy_after);
}

void ribi::cmap::QtCommandSelectNodeTest
  ::SelectTwoQtNodesByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetThreeNodeTwoEdgeNoCenter());

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  assert(!GetQtToolItemBuddy(q));

  QtNode * const qtnode1 = FindFirstQtNode(q, QtNodeHasName("one"));
  q.DoCommand(new CommandSelectNode(q, qtnode1));

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 1);
  assert(GetQtToolItemBuddy(q) == qtnode1);

  QtNode * const qtnode2 = FindFirstQtNode(q, QtNodeHasName("two"));
  q.DoCommand(new CommandSelectNode(q, qtnode2));

  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 2);
  QVERIFY(GetQtToolItemBuddy(q) == qtnode2);
}

void ribi::cmap::QtCommandSelectNodeTest
  ::SelectTwoQtNodesByNameAndUndo() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetThreeNodeTwoEdgeNoCenter());

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  assert(!GetQtToolItemBuddy(q));

  QtNode * const qtnode1 = FindFirstQtNode(q, QtNodeHasName("one"));
  q.DoCommand(new CommandSelectNode(q, qtnode1));

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 1);
  assert(GetQtToolItemBuddy(q) == qtnode1);

  QtNode * const qtnode2 = FindFirstQtNode(q, QtNodeHasName("two"));
  q.DoCommand(new CommandSelectNode(q, qtnode2));

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 2);
  assert(GetQtToolItemBuddy(q) == qtnode2);

  q.Undo();

  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 1);
  QVERIFY(GetQtToolItemBuddy(q) == qtnode1);

  q.Undo();

  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 0);
  QVERIFY(!GetQtToolItemBuddy(q));
}

void ribi::cmap::QtCommandSelectNodeTest::Parse() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  const auto c = ParseCommandSelectNode(q, "select_node(one)");
  assert(c);
  QVERIFY(c != nullptr);
}

void ribi::cmap::QtCommandSelectNodeTest::ParseNonsenseFails() const noexcept
{
  QtConceptMap q;
  QVERIFY(ParseCommandSelectNode(q, "nonsense") == nullptr);
}
