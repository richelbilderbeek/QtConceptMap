#include "qtconceptmapcommandunselectnode_test.h"

#include "qtconceptmapcommandunselectnode.h"
#include "qtconceptmapcommandselectnode.h"
#include "qtconceptmapcommandcreatenewnode.h"
#include "qtconceptmap.h"
#include "qtconceptmapqtnode.h"
#include "conceptmapfactory.h"

void ribi::cmap::QtCommandUnselectNodeTest::UnselectAbsentItemFails() const noexcept
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

void ribi::cmap::QtCommandUnselectNodeTest::UnselectLonelyQtCenterNodeByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetLonelyQtCenterNode());
  QtNode * const first_qtnode = FindFirstQtNode(q, QtNodeHasName("center"));
  q.DoCommand(new CommandSelectNode(q, first_qtnode));
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 1);
  q.DoCommand(new CommandUnselectNode(q, first_qtnode));

  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 0);
}

void ribi::cmap::QtCommandUnselectNodeTest::UnselectQtCenterNodeByName() const noexcept
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

void ribi::cmap::QtCommandUnselectNodeTest
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

void ribi::cmap::QtCommandUnselectNodeTest
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
  assert(HasExamples(*first_qtnode));
  assert(GetQtToolItemBuddy(q) == first_qtnode);

  q.DoCommand(new CommandUnselectNode(q, first_qtnode));

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  assert(!GetQtToolItemBuddy(q));

  q.Undo();

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 1);
  assert(HasExamples(*first_qtnode));
  assert(GetQtToolItemBuddy(q) == first_qtnode);
}

void ribi::cmap::QtCommandUnselectNodeTest
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

void ribi::cmap::QtCommandUnselectNodeTest
  ::UnselectTwoQtNodesByNameAndUndo() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdgeNoCenter());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  QtNode * const qtnode1 = FindFirstQtNode(q, QtNodeHasName("one"));
  QtNode * const qtnode2 = FindFirstQtNode(q, QtNodeHasName("two"));
  q.DoCommand(new CommandSelectNode(q, qtnode1));

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 1);
  assert(GetQtToolItemBuddy(q) == qtnode1);

  q.DoCommand(new CommandSelectNode(q, qtnode2));

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 2);
  assert(GetQtToolItemBuddy(q) == qtnode2);

  q.DoCommand(new CommandUnselectNode(q, qtnode1));

  //QtTool and QtExamplesItem do not know where to go
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 1);
  assert(GetQtToolItemBuddy(q) == nullptr);

  q.DoCommand(new CommandUnselectNode(q, qtnode2));

  //QtTool does not know where to go
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  assert(GetQtToolItemBuddy(q) == nullptr);

  q.Undo();

  //QtTool does not know where to go
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 1);
  assert(GetQtToolItemBuddy(q) == nullptr);

  q.Undo();

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 2);
  assert(GetQtToolItemBuddy(q) == qtnode2);
}

void ribi::cmap::QtCommandUnselectNodeTest::Parse() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  QtNode * const first_qtnode = FindFirstQtNode(q, QtNodeHasName("one"));
  q.DoCommand(new CommandSelectNode(q, first_qtnode));

  const auto c = ParseCommandUnselectNode(q, "unselect_node(one)");
  assert(c);
  QVERIFY(c != nullptr);
}

void ribi::cmap::QtCommandUnselectNodeTest::ParseNonsenseFails() const noexcept
{
  QtConceptMap q;
  QVERIFY(ParseCommandUnselectNode(q, "nonsense") == nullptr);
}
