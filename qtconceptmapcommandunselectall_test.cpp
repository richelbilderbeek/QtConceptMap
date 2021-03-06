#include "qtconceptmapcommandunselectall_test.h"

#include "qtconceptmapcommandunselectall.h"
#include "qtconceptmap.h"

#include "qtconceptmapcommandselect.h"
#include "qtconceptmapcommandcreatenewnode.h"
#include "qtconceptmapcommandcreatenewedge.h"
#include "qtconceptmap.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmapqtedge.h"
#include "conceptmapfactory.h"

void ribi::cmap::QtCommandUnselectAllTest::UnselectAllAbsentItemFails() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  try
  {
    q.DoCommand(new CommandUnselectAll(q));
    QVERIFY(!"Should not get here");
  }
  catch (std::exception&)
  {
    QVERIFY("OK");
  }
}

void ribi::cmap::QtCommandUnselectAllTest::UnselectAllQtCenterEdgeByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  QtEdge * const first_qtedge = FindFirstQtEdge(q, QtEdgeHasName("first"));
  q.DoCommand(new CommandSelect(q, *first_qtedge));
  assert(CountSelectedQtEdges(q) == 1);
  assert(CountSelectedQtNodes(q) == 0);
  q.DoCommand(new CommandUnselectAll(q));

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
}

void ribi::cmap::QtCommandUnselectAllTest::UnselectAllOneOfTwoQtEdgesByName() const noexcept
{
  QtConceptMap q;
  q.DoCommand(new CommandCreateNewNode(q));
  q.DoCommand(new CommandCreateNewNode(q));
  q.DoCommand(new CommandCreateNewEdge(q));

  assert(CountSelectedQtEdges(q) == 1);
  assert(CountSelectedQtNodes(q) == 0);

  q.DoCommand(new CommandUnselectAll(q));

  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 0);
}

void ribi::cmap::QtCommandUnselectAllTest
  ::UnselectAllOneOfTwoQtEdgesByNameAndUndo() const noexcept
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

  q.DoCommand(new CommandUnselectAll(q));

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  assert(!GetQtToolItemBuddy(q));

  q.Undo();

  assert(CountSelectedQtEdges(q) == 1);
  assert(CountSelectedQtNodes(q) == 0);
  assert(HasExamples(*first_qtedge));
  assert(GetQtToolItemBuddy(q) == nullptr);
}

void ribi::cmap::QtCommandUnselectAllTest
  ::UnselectAllTwoQtEdgesByName() const noexcept
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

  q.DoCommand(new CommandUnselectAll(q));

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
}

void ribi::cmap::QtCommandUnselectAllTest
  ::UnselectAllTwoQtEdgesByNameAndUndo() const noexcept
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

  q.DoCommand(new CommandUnselectAll(q));

  //QtTool and QtExamplesItem do not know where to go
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  assert(GetQtToolItemBuddy(q) == nullptr);

  q.Undo();

  assert(CountSelectedQtEdges(q) == 2);
  assert(CountSelectedQtNodes(q) == 0);
  assert(GetQtToolItemBuddy(q) == nullptr);
}

void ribi::cmap::QtCommandUnselectAllTest::UnselectAllQtCenterNodeByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  QtNode * const first_qtnode = FindFirstQtNode(q, QtNodeHasName("center"));
  q.DoCommand(new CommandSelect(q, *first_qtnode));
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 1);
  q.DoCommand(new CommandUnselectAll(q));

  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 0);
}

void ribi::cmap::QtCommandUnselectAllTest
  ::UnselectAllOneOfTwoQtNodesByName() const noexcept
{
  QtConceptMap q;
  q.DoCommand(new CommandCreateNewNode(q));

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 1);

  q.DoCommand(new CommandUnselectAll(q));

  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 0);
}

void ribi::cmap::QtCommandUnselectAllTest
  ::UnselectAllOneOfTwoQtNodesByNameAndUndo() const noexcept
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

  q.DoCommand(new CommandUnselectAll(q));

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  assert(!GetQtToolItemBuddy(q));

  q.Undo();

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 1);
  assert(HasExamples(*first_qtnode));
  assert(GetQtToolItemBuddy(q) == first_qtnode);
}

void ribi::cmap::QtCommandUnselectAllTest
  ::UnselectAllTwoQtNodesByName() const noexcept
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

  q.DoCommand(new CommandUnselectAll(q));

  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 0);
}

void ribi::cmap::QtCommandUnselectAllTest
  ::UnselectAllTwoQtNodesByNameAndUndo() const noexcept
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

  q.DoCommand(new CommandUnselectAll(q));

  //QtTool and QtExamplesItem do not know where to go
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  assert(GetQtToolItemBuddy(q) == nullptr);

  q.Undo();

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 2);
  assert(GetQtToolItemBuddy(q) == qtnode2);
}

void ribi::cmap::QtCommandUnselectAllTest::Parse() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  QtNode * const first_qtnode = FindFirstQtNode(q, QtNodeHasName("one"));
  q.DoCommand(new CommandSelect(q, *first_qtnode));

  const auto c = ParseCommandUnselectAll(q, "unselect_all()");
  assert(c);
  QVERIFY(c != nullptr);
}

void ribi::cmap::QtCommandUnselectAllTest::ParseNonsenseFails() const noexcept
{
  QtConceptMap q;
  QVERIFY(ParseCommandUnselectAll(q, "nonsense") == nullptr);
}
