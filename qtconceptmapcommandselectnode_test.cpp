#include "qtconceptmapcommandselectnode_test.h"

#include "qtconceptmapcommandselectnode.h"
#include "qtconceptmap.h"
#include "conceptmapfactory.h"

void ribi::cmap::QtConceptMapCommandSelectNodeTest::SelectAbsentItemFails() const noexcept
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

void ribi::cmap::QtConceptMapCommandSelectNodeTest::SelectQtCenterNodeByName() const noexcept
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

void ribi::cmap::QtConceptMapCommandSelectNodeTest
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

void ribi::cmap::QtConceptMapCommandSelectNodeTest
  ::SelectOneOfTwoQtNodesByNameAndUndo() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);

  const auto examples_buddy_before = GetQtExamplesItemBuddy(q);
  const auto tool_buddy_before = GetQtToolItemBuddy(q);

  QtNode * const first_qtnode = FindFirstQtNode(q, QtNodeHasName("one"));
  q.DoCommand(new CommandSelectNode(q, first_qtnode));

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 1);
  assert(HasExamples(*first_qtnode));
  assert(examples_buddy_before != GetQtExamplesItemBuddy(q));
  assert(tool_buddy_before != GetQtToolItemBuddy(q));

  q.Undo();

  const auto examples_buddy_after = GetQtExamplesItemBuddy(q);
  const auto tool_buddy_after = GetQtToolItemBuddy(q);

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  QVERIFY(examples_buddy_before == examples_buddy_after);
  QVERIFY(tool_buddy_before == tool_buddy_after);
}

void ribi::cmap::QtConceptMapCommandSelectNodeTest
  ::SelectTwoQtNodesByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  QtNode * const center_qtnode = FindFirstQtNode(q, QtNodeHasName("center"));
  QtNode * const normal_qtnode = FindFirstQtNode(q, QtNodeHasName("one"));
  q.DoCommand(new CommandSelectNode(q, center_qtnode));
  q.DoCommand(new CommandSelectNode(q, normal_qtnode));
  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 2);
}

void ribi::cmap::QtConceptMapCommandSelectNodeTest
  ::SelectTwoQtNodesByNameAndUndo() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  QtNode * const center_qtnode = FindFirstQtNode(q, QtNodeHasName("center"));
  QtNode * const normal_qtnode = FindFirstQtNode(q, QtNodeHasName("one"));

  q.DoCommand(new CommandSelectNode(q, normal_qtnode));

  const auto examples_buddy_before = GetQtExamplesItemBuddy(q);
  const auto tool_buddy_before = GetQtToolItemBuddy(q);

  q.DoCommand(new CommandSelectNode(q, center_qtnode));

  assert(examples_buddy_before != GetQtExamplesItemBuddy(q));
  assert(tool_buddy_before != GetQtToolItemBuddy(q));

  q.Undo();

  const auto examples_buddy_after = GetQtExamplesItemBuddy(q);
  const auto tool_buddy_after = GetQtToolItemBuddy(q);
  QVERIFY(examples_buddy_before == examples_buddy_after);
  QVERIFY(tool_buddy_before == tool_buddy_after);
}

void ribi::cmap::QtConceptMapCommandSelectNodeTest::Parse() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  const auto c = ParseCommandSelectNode(q, "select_node(one)");
  assert(c);
  QVERIFY(c != nullptr);
}

void ribi::cmap::QtConceptMapCommandSelectNodeTest::ParseNonsenseFails() const noexcept
{
  QtConceptMap q;
  QVERIFY(ParseCommandSelectNode(q, "nonsense") == nullptr);
}
