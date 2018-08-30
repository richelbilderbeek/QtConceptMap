#include "qtconceptmapcommanddeleteselected_test.h"

#include "qtconceptmapcommanddeleteselected.h"
#include "qtconceptmapcommands.h"
#include "qtconceptmap.h"
#include "conceptmapfactory.h"
#include "qtconceptmaphelper.h"
#include "qtconceptmapcommandselectedge.h"
#include "qtconceptmapcommandselectnode.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmapqtedge.h"

void ribi::cmap::QtCommandDeleteSelectedTest::DeleteSelectedCenterNodeThrows() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetLonelyCenterNode());
  QtNode * const first_qtnode = FindFirstQtNode(q, QtNodeHasName("center"));
  assert(IsQtCenterNode(*first_qtnode));
  q.DoCommand(new CommandSelectNode(q, first_qtnode));
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 1);
  assert(CountQtNodes(q) == 1);
  try
  {
    q.DoCommand(new CommandDeleteSelected(q));
    assert(!"Should not get here");
  }
  catch (const std::invalid_argument&)
  {
    QVERIFY("Should get here");
  }

}

void ribi::cmap::QtCommandDeleteSelectedTest::DeleteSelectedEdge() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  QtEdge * const first_qtedge = GetFirstQtEdge(q);
  q.DoCommand(new CommandSelectEdge(q, first_qtedge));
  assert(CountSelectedQtEdges(q) == 1);
  assert(CountSelectedQtNodes(q) == 0);
  assert(CountQtEdges(q) == 1);
  q.DoCommand(new CommandDeleteSelected(q));
  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountQtEdges(q) == 0);
}

void ribi::cmap::QtCommandDeleteSelectedTest::DeleteSelectedEdges() const noexcept
{
  //Delete
  // - 1 selected edge
  // - 1 selected node
  // - 2 edges connected to the selected node
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetStarShaped());
  const auto qtnode = FindFirstQtNode(q, QtNodeHasName("A"));
  const auto qtedge = FindFirstQtEdge(q, QtEdgeHasName("2"));
  SetSelectedness(true, *qtnode);
  SetSelectedness(true, *qtedge);
  assert(CountSelectedQtEdges(q) == 1);
  assert(CountSelectedQtNodes(q) == 1);
  assert(CountQtNodes(q) == 5);
  assert(CountQtEdges(q) == 3);
  q.DoCommand(new CommandDeleteSelected(q));
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  assert(CountQtNodes(q) == 4);
  QVERIFY(CountQtEdges(q) == 0);
}

void ribi::cmap::QtCommandDeleteSelectedTest::DeleteSelectedNode() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  QtNode * const first_qtnode = FindFirstQtNode(q, QtNodeHasName("one"));
  assert(!IsQtCenterNode(first_qtnode));
  q.DoCommand(new CommandSelectNode(q, first_qtnode));
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 1);
  assert(CountQtNodes(q) == 2);
  q.DoCommand(new CommandDeleteSelected(q));
  QVERIFY(CountSelectedQtNodes(q) == 0);
  QVERIFY(CountQtNodes(q) == 1);
}
