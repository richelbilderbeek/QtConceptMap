#include "qtconceptmapcommanddeleteselected_test.h"

#include "qtconceptmapcommanddeleteselected.h"
#include "qtconceptmapcommands.h"
#include "qtconceptmap.h"
#include "conceptmapfactory.h"
#include "qtconceptmaphelper.h"
#include "qtconceptmapcommandselectedge.h"
#include "qtconceptmapcommandselectnode.h"
#include "qtconceptmapqtnode.h"

void ribi::cmap::QtCommandDeleteSelectedTest::DeleteSelectedCenterNodeThrows() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetLonelyQtCenterNode());
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
