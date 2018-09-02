#include "qtconceptmapcommandunselect_test.h"

#include "qtconceptmapcommandunselect.h"
#include "qtconceptmap.h"

#include "qtconceptmapcommandunselectnode.h"
#include "qtconceptmapcommandselect.h"
#include "qtconceptmapcommandcreatenewnode.h"
#include "qtconceptmapcommandcreatenewedge.h"
#include "qtconceptmap.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmapqtedge.h"
#include "conceptmapfactory.h"

void ribi::cmap::QtCommandUnselectTest::UnselectLonelyQtCenterNodeByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetLonelyCenterNode());
  QtNode * const first_qtnode = FindFirstQtNode(q, QtNodeHasName("center"));
  q.DoCommand(new CommandSelect(q, *first_qtnode));
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 1);
  q.DoCommand(new CommandUnselectNode(q, first_qtnode));

  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 0);
}

void ribi::cmap::QtCommandUnselectTest::UnselectQtCenterEdgeByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  QtEdge * const first_qtedge = FindFirstQtEdge(q, QtEdgeHasName("first"));
  q.DoCommand(new CommandSelect(q, *first_qtedge));
  assert(CountSelectedQtEdges(q) == 1);
  assert(CountSelectedQtNodes(q) == 0);
  q.DoCommand(new CommandUnselect(q, *first_qtedge));

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
}

void ribi::cmap::QtCommandUnselectTest
  ::UnselectOneOfTwoQtEdgesByName() const noexcept
{
  QtConceptMap q;
  q.DoCommand(new CommandCreateNewNode(q));
  q.DoCommand(new CommandCreateNewNode(q));
  q.DoCommand(new CommandCreateNewEdge(q));

  assert(CountSelectedQtEdges(q) == 1);
  assert(CountSelectedQtNodes(q) == 0);

  q.DoCommand(new CommandUnselect(q, *GetFirstQtEdge(q)));

  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 0);
}

void ribi::cmap::QtCommandUnselectTest
  ::UnselectOneOfTwoQtEdgesByNameAndUndo() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetThreeNodeTwoEdge());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  QtEdge * const first_qtedge = FindFirstQtEdge(q, QtEdgeHasName("first"));
  q.DoCommand(new CommandSelect(q, *first_qtedge));

  assert(CountSelectedQtEdges(q) == 1);
  assert(CountSelectedQtNodes(q) == 0);
  assert(HasExamples(*first_qtedge));
  assert(GetQtToolItemBuddy(q) == nullptr);

  q.DoCommand(new CommandUnselect(q, *first_qtedge));

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  assert(!GetQtToolItemBuddy(q));

  q.Undo();

  assert(CountSelectedQtEdges(q) == 1);
  assert(CountSelectedQtNodes(q) == 0);
  assert(HasExamples(*first_qtedge));
  assert(GetQtToolItemBuddy(q) == nullptr);
}

void ribi::cmap::QtCommandUnselectTest
  ::UnselectTwoQtEdgesByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetThreeNodeTwoEdge());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  QtEdge * const center_qtnode = FindFirstQtEdge(q, QtEdgeHasName("first"));
  QtEdge * const normal_qtnode = FindFirstQtEdge(q, QtEdgeHasName("second"));

  q.DoCommand(new CommandSelect(q, *center_qtnode));
  q.DoCommand(new CommandSelect(q, *normal_qtnode));

  assert(CountSelectedQtEdges(q) == 2);
  assert(CountSelectedQtNodes(q) == 0);

  q.DoCommand(new CommandUnselect(q, *center_qtnode));
  q.DoCommand(new CommandUnselect(q, *normal_qtnode));

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
}

void ribi::cmap::QtCommandUnselectTest
  ::UnselectTwoQtEdgesByNameAndUndo() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetThreeNodeTwoEdgeNoCenter());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  QtEdge * const qtedge1 = FindFirstQtEdge(q, QtEdgeHasName("first"));
  QtEdge * const qtedge2 = FindFirstQtEdge(q, QtEdgeHasName("second"));
  q.DoCommand(new CommandSelect(q, *qtedge1));

  assert(CountSelectedQtEdges(q) == 1);
  assert(CountSelectedQtNodes(q) == 0);
  assert(GetQtToolItemBuddy(q) == nullptr);

  q.DoCommand(new CommandSelect(q, *qtedge2));

  assert(CountSelectedQtEdges(q) == 2);
  assert(CountSelectedQtNodes(q) == 0);
  assert(GetQtToolItemBuddy(q) == nullptr);

  q.DoCommand(new CommandUnselect(q, *qtedge1));

  //QtTool and QtExamplesItem do not know where to go
  assert(CountSelectedQtEdges(q) == 1);
  assert(CountSelectedQtNodes(q) == 0);
  assert(GetQtToolItemBuddy(q) == nullptr);

  q.DoCommand(new CommandUnselect(q, *qtedge2));

  //QtTool and QtExamplesItem do not know where to go
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  assert(GetQtToolItemBuddy(q) == nullptr);

  q.Undo();

  //QtTool and QtExamplesItem do not know where to go
  assert(CountSelectedQtEdges(q) == 1);
  assert(CountSelectedQtNodes(q) == 0);
  assert(GetQtToolItemBuddy(q) == nullptr);

  q.Undo();

  assert(CountSelectedQtEdges(q) == 2);
  assert(CountSelectedQtNodes(q) == 0);
  assert(GetQtToolItemBuddy(q) == nullptr);
}

void ribi::cmap::QtCommandUnselectTest::UnselectQtCenterNodeByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  QtNode * const first_qtnode = FindFirstQtNode(q, QtNodeHasName("center"));
  q.DoCommand(new CommandSelect(q, *first_qtnode));
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 1);
  q.DoCommand(new CommandUnselect(q, *first_qtnode));

  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 0);
}

void ribi::cmap::QtCommandUnselectTest
  ::UnselectOneOfTwoQtNodesByName() const noexcept
{
  QtConceptMap q;
  q.DoCommand(new CommandCreateNewNode(q));

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 1);

  q.DoCommand(new CommandUnselect(q, *GetFirstQtNode(q)));

  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 0);
}

void ribi::cmap::QtCommandUnselectTest
  ::UnselectOneOfTwoQtNodesByNameAndUndo() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  QtNode * const first_qtnode = FindFirstQtNode(q, QtNodeHasName("one"));
  q.DoCommand(new CommandSelect(q, *first_qtnode));

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 1);
  assert(HasExamples(*first_qtnode));
  assert(GetQtToolItemBuddy(q) == first_qtnode);

  q.DoCommand(new CommandUnselect(q, *first_qtnode));

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  assert(!GetQtToolItemBuddy(q));

  q.Undo();

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 1);
  assert(HasExamples(*first_qtnode));
  assert(GetQtToolItemBuddy(q) == first_qtnode);
}

void ribi::cmap::QtCommandUnselectTest
  ::UnselectTwoQtNodesByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  QtNode * const center_qtnode = FindFirstQtNode(q, QtNodeHasName("center"));
  QtNode * const normal_qtnode = FindFirstQtNode(q, QtNodeHasName("one"));

  q.DoCommand(new CommandSelect(q, *center_qtnode));
  q.DoCommand(new CommandSelect(q, *normal_qtnode));

  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 2);

  q.DoCommand(new CommandUnselect(q, *center_qtnode));
  q.DoCommand(new CommandUnselect(q, *normal_qtnode));

  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 0);
}

void ribi::cmap::QtCommandUnselectTest
  ::UnselectTwoQtNodesByNameAndUndo() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdgeNoCenter());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  QtNode * const qtnode1 = FindFirstQtNode(q, QtNodeHasName("one"));
  QtNode * const qtnode2 = FindFirstQtNode(q, QtNodeHasName("two"));
  q.DoCommand(new CommandSelect(q, *qtnode1));

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 1);
  assert(GetQtToolItemBuddy(q) == qtnode1);

  q.DoCommand(new CommandSelect(q, *qtnode2));

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 2);
  assert(GetQtToolItemBuddy(q) == qtnode2);

  q.DoCommand(new CommandUnselect(q, *qtnode1));

  //QtTool and QtExamplesItem do not know where to go
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 1);
  assert(GetQtToolItemBuddy(q) == nullptr);

  q.DoCommand(new CommandUnselect(q, *qtnode2));

  //QtTool and QtExamplesItem do not know where to go
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  assert(GetQtToolItemBuddy(q) == nullptr);

  q.Undo();

  //QtTool and QtExamplesItem do not know where to go
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 1);
  assert(GetQtToolItemBuddy(q) == nullptr);

  q.Undo();

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 2);
  assert(GetQtToolItemBuddy(q) == qtnode2);
}

void ribi::cmap::QtCommandUnselectTest::ParseNode() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  QtNode * const first_qtnode = FindFirstQtNode(q, QtNodeHasName("one"));
  q.DoCommand(new CommandSelect(q, *first_qtnode));

  const auto c = ParseCommandUnselect(q, "unselect(one)");
  assert(c);
  QVERIFY(c != nullptr);
}

void ribi::cmap::QtCommandUnselectTest::ParseNodeOnEdge() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(GetText(*GetFirstQtEdge(q)) == "first");
  SetSelectedness(true, *GetFirstQtEdge(q));
  assert(IsSelected(*GetFirstQtEdge(q)));
  const auto c = ParseCommandUnselect(q, "unselect(first)");
  assert(c);
  QVERIFY(c != nullptr);
}

void ribi::cmap::QtCommandUnselectTest::ParseNonsenseFails() const noexcept
{
  QtConceptMap q;
  QVERIFY(ParseCommandUnselect(q, "nonsense") == nullptr);
}
