#include "qtconceptmapcommandunselectedge_test.h"

#include "qtconceptmapcommandunselectedge.h"
#include "qtconceptmapcommandselectedge.h"
#include "qtconceptmapcommandcreatenewedge.h"
#include "qtconceptmapcommandcreatenewnode.h"
#include "qtconceptmap.h"
#include "qtconceptmapqtedge.h"
#include "conceptmapfactory.h"

void ribi::cmap::QtConceptMapCommandUnselectEdgeTest::UnselectAbsentItemFails() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  try
  {
    QtEdge * const first_qtedge = FindFirstQtEdge(q, QtEdgeHasName("absent"));
    q.DoCommand(new CommandUnselectEdge(q, first_qtedge));
    QVERIFY(!"Should not get here");
  }
  catch (std::exception&)
  {
    QVERIFY("OK");
  }
}

void ribi::cmap::QtConceptMapCommandUnselectEdgeTest::UnselectQtCenterEdgeByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  QtEdge * const first_qtedge = FindFirstQtEdge(q, QtEdgeHasName("first"));
  q.DoCommand(new CommandSelectEdge(q, first_qtedge));
  assert(CountSelectedQtEdges(q) == 1);
  assert(CountSelectedQtNodes(q) == 0);
  q.DoCommand(new CommandUnselectEdge(q, first_qtedge));

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
}

void ribi::cmap::QtConceptMapCommandUnselectEdgeTest
  ::UnselectOneOfTwoQtEdgesByName() const noexcept
{
  QtConceptMap q;
  q.DoCommand(new CommandCreateNewNode(q));
  q.DoCommand(new CommandCreateNewNode(q));
  q.DoCommand(new CommandCreateNewEdgeBetweenTwoSelectedNodes(q));

  assert(CountSelectedQtEdges(q) == 1);
  assert(CountSelectedQtNodes(q) == 0);

  q.DoCommand(new CommandUnselectEdge(q, GetFirstQtEdge(q)));

  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 0);
}

void ribi::cmap::QtConceptMapCommandUnselectEdgeTest
  ::UnselectOneOfTwoQtEdgesByNameAndUndo() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetThreeNodeTwoEdge());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  QtEdge * const first_qtedge = FindFirstQtEdge(q, QtEdgeHasName("first"));
  q.DoCommand(new CommandSelectEdge(q, first_qtedge));

  assert(CountSelectedQtEdges(q) == 1);
  assert(CountSelectedQtNodes(q) == 0);
  assert(HasExamples(*first_qtedge) && GetQtExamplesItemBuddy(q) == first_qtedge->GetQtNode());
  assert(GetQtToolItemBuddy(q) == first_qtedge->GetQtNode());

  q.DoCommand(new CommandUnselectEdge(q, first_qtedge));

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  assert(!GetQtExamplesItemBuddy(q));
  assert(!GetQtToolItemBuddy(q));

  q.Undo();

  assert(CountSelectedQtEdges(q) == 1);
  assert(CountSelectedQtNodes(q) == 0);
  assert(HasExamples(*first_qtedge) && GetQtExamplesItemBuddy(q) == first_qtedge->GetQtNode());
  assert(GetQtToolItemBuddy(q) == first_qtedge->GetQtNode());
}

void ribi::cmap::QtConceptMapCommandUnselectEdgeTest
  ::UnselectTwoQtEdgesByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetThreeNodeTwoEdge());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  QtEdge * const center_qtnode = FindFirstQtEdge(q, QtEdgeHasName("first"));
  QtEdge * const normal_qtnode = FindFirstQtEdge(q, QtEdgeHasName("second"));

  q.DoCommand(new CommandSelectEdge(q, center_qtnode));
  q.DoCommand(new CommandSelectEdge(q, normal_qtnode));

  assert(CountSelectedQtEdges(q) == 2);
  assert(CountSelectedQtNodes(q) == 0);

  q.DoCommand(new CommandUnselectEdge(q, center_qtnode));
  q.DoCommand(new CommandUnselectEdge(q, normal_qtnode));

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
}

void ribi::cmap::QtConceptMapCommandUnselectEdgeTest
  ::UnselectTwoQtEdgesByNameAndUndo() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetThreeNodeTwoEdgeNoCenter());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  QtEdge * const qtnode1 = FindFirstQtEdge(q, QtEdgeHasName("first"));
  QtEdge * const qtnode2 = FindFirstQtEdge(q, QtEdgeHasName("second"));
  q.DoCommand(new CommandSelectEdge(q, qtnode1));

  assert(CountSelectedQtEdges(q) == 1);
  assert(CountSelectedQtNodes(q) == 0);
  assert(GetQtToolItemBuddy(q) == qtnode1->GetQtNode());
  assert(GetQtExamplesItemBuddy(q) == qtnode1->GetQtNode());

  q.DoCommand(new CommandSelectEdge(q, qtnode2));

  assert(CountSelectedQtEdges(q) == 2);
  assert(CountSelectedQtNodes(q) == 0);
  assert(GetQtToolItemBuddy(q) == qtnode2->GetQtNode());
  assert(GetQtExamplesItemBuddy(q) == qtnode2->GetQtNode());

  q.DoCommand(new CommandUnselectEdge(q, qtnode1));

  //QtTool and QtExamplesItem do not know where to go
  assert(CountSelectedQtEdges(q) == 1);
  assert(CountSelectedQtNodes(q) == 0);
  assert(GetQtToolItemBuddy(q) == nullptr);
  assert(GetQtExamplesItemBuddy(q) == nullptr);

  q.DoCommand(new CommandUnselectEdge(q, qtnode2));

  //QtTool and QtExamplesItem do not know where to go
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  assert(GetQtToolItemBuddy(q) == nullptr);
  assert(GetQtExamplesItemBuddy(q) == nullptr);

  q.Undo();

  //QtTool and QtExamplesItem do not know where to go
  assert(CountSelectedQtEdges(q) == 1);
  assert(CountSelectedQtNodes(q) == 0);
  assert(GetQtToolItemBuddy(q) == nullptr);
  assert(GetQtExamplesItemBuddy(q) == nullptr);

  q.Undo();

  assert(CountSelectedQtEdges(q) == 2);
  assert(CountSelectedQtNodes(q) == 0);
  assert(GetQtToolItemBuddy(q) == qtnode2->GetQtNode());
  assert(GetQtExamplesItemBuddy(q) == qtnode2->GetQtNode());
}

void ribi::cmap::QtConceptMapCommandUnselectEdgeTest::Parse() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  QtEdge * const first_qtedge = FindFirstQtEdge(q, QtEdgeHasName("first"));
  q.DoCommand(new CommandSelectEdge(q, first_qtedge));

  const auto c = ParseCommandUnselectEdge(q, "unselect_edge(first)");
  assert(c);
  QVERIFY(c != nullptr);
}

void ribi::cmap::QtConceptMapCommandUnselectEdgeTest::ParseNonsenseFails() const noexcept
{
  QtConceptMap q;
  QVERIFY(ParseCommandUnselectEdge(q, "nonsense") == nullptr);
}