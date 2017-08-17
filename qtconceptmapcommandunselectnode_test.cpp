#include "qtconceptmapcommandunselectnode_test.h"

#include "qtconceptmapcommandunselectnode.h"
#include "qtconceptmapcommandselectnode.h"
#include "qtconceptmapcommandcreatenewnode.h"
#include "qtconceptmap.h"
#include "conceptmapfactory.h"

void ribi::cmap::QtConceptMapCommandUnselectNodeTest::UnselectAbsentItemFails() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  try
  {
    QtNode * const first_qtnode = FindFirstQtNode(q, QtNodeHasName("absent"));
    q.DoCommand(new CommandUnselectNode(q, first_qtnode));
    QVERIFY(!"Should not get here");
  }
  catch (std::exception&)
  {
    QVERIFY("OK");
  }
}

void ribi::cmap::QtConceptMapCommandUnselectNodeTest::UnselectQtCenterNodeByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  QtNode * const first_qtnode = FindFirstQtNode(q, QtNodeHasName("center"));
  q.DoCommand(new CommandSelectNode(q, first_qtnode));
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 1);
  q.DoCommand(new CommandUnselectNode(q, first_qtnode));

  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 0);
}

void ribi::cmap::QtConceptMapCommandUnselectNodeTest
  ::UnselectOneOfTwoQtNodesByName() const noexcept
{
  QtConceptMap q;
  q.DoCommand(new CommandCreateNewNode(q));

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 1);

  q.DoCommand(new CommandUnselectNode(q, GetFirstQtNode(q)));

  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 0);
}

void ribi::cmap::QtConceptMapCommandUnselectNodeTest
  ::UnselectOneOfTwoQtNodesByNameAndUndo() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  QtNode * const first_qtnode = FindFirstQtNode(q, QtNodeHasName("one"));
  q.DoCommand(new CommandSelectNode(q, first_qtnode));

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 1);

  const auto examples_buddy_before = GetQtExamplesItemBuddy(q);
  const auto tool_buddy_before = GetQtToolItemBuddy(q);

  q.DoCommand(new CommandUnselectNode(q, first_qtnode));

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  assert(HasExamples(*first_qtnode));
  assert(examples_buddy_before != GetQtExamplesItemBuddy(q));
  assert(tool_buddy_before != GetQtToolItemBuddy(q));

  q.Undo();

  const auto examples_buddy_after = GetQtExamplesItemBuddy(q);
  const auto tool_buddy_after = GetQtToolItemBuddy(q);

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 1);
  QVERIFY(examples_buddy_before == examples_buddy_after);
  QVERIFY(tool_buddy_before == tool_buddy_after);
}

void ribi::cmap::QtConceptMapCommandUnselectNodeTest
  ::UnselectTwoQtNodesByName() const noexcept
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

  q.DoCommand(new CommandUnselectNode(q, center_qtnode));
  q.DoCommand(new CommandUnselectNode(q, normal_qtnode));

  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 0);
}

void ribi::cmap::QtConceptMapCommandUnselectNodeTest
  ::UnselectTwoQtNodesByNameAndUndo() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  QtNode * const center_qtnode = FindFirstQtNode(q, QtNodeHasName("center"));
  QtNode * const normal_qtnode = FindFirstQtNode(q, QtNodeHasName("one"));
  q.DoCommand(new CommandSelectNode(q, normal_qtnode));
  q.DoCommand(new CommandSelectNode(q, center_qtnode));

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 2);

  q.DoCommand(new CommandUnselectNode(q, center_qtnode));

  const auto examples_buddy_before = GetQtExamplesItemBuddy(q);
  const auto tool_buddy_before = GetQtToolItemBuddy(q);

  assert(GetQtToolItemBuddy(q) == GetFirstQtNode(q));
  assert(GetQtExamplesItemBuddy(q) == GetFirstQtNode(q));
  assert(GetQtToolItemBuddy(q) == GetFirstQtNode(q));

  q.DoCommand(new CommandUnselectNode(q, normal_qtnode));

  assert(examples_buddy_before != GetQtExamplesItemBuddy(q));
  assert(tool_buddy_before != GetQtToolItemBuddy(q));

  q.Undo();

  const auto examples_buddy_after = GetQtExamplesItemBuddy(q);
  const auto tool_buddy_after = GetQtToolItemBuddy(q);
  QVERIFY(examples_buddy_before == examples_buddy_after);
  QVERIFY(tool_buddy_before == tool_buddy_after);
}

void ribi::cmap::QtConceptMapCommandUnselectNodeTest::Parse() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  QtNode * const first_qtnode = FindFirstQtNode(q, QtNodeHasName("one"));
  q.DoCommand(new CommandSelectNode(q, first_qtnode));

  const auto c = ParseCommandUnselectNode(q, "unselect_node(one)");
  assert(c);
  QVERIFY(c != nullptr);
}

void ribi::cmap::QtConceptMapCommandUnselectNodeTest::ParseNonsenseFails() const noexcept
{
  QtConceptMap q;
  QVERIFY(ParseCommandUnselectNode(q, "nonsense") == nullptr);
}
