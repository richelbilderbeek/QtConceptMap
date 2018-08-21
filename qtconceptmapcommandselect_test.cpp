#include "qtconceptmapcommandselect_test.h"

#include "qtconceptmapcommandselect.h"
#include "qtconceptmap.h"
#include "conceptmapfactory.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmapqtnode.h"

void ribi::cmap::QtCommandSelectTest::SelectQtCenterNode() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(CountSelectedQtNodes(q) == 0);
  q.DoCommand(new CommandSelect(q, *FindFirstQtNode(q, QtNodeHasName("center"))));
  QVERIFY(CountSelectedQtNodes(q) == 1);
}

void ribi::cmap::QtCommandSelectTest::SelectQtEdge() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdgeNoCenter());
  assert(CountSelectedQtEdges(q) == 0);
  q.DoCommand(new CommandSelect(q, *GetFirstQtEdge(q)));
  QVERIFY(CountSelectedQtEdges(q) == 1);
}

void ribi::cmap::QtCommandSelectTest::SelectQtEdgeAndUndo() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdgeNoCenter());
  assert(CountSelectedQtEdges(q) == 0);
  q.DoCommand(new CommandSelect(q, *GetFirstQtEdge(q)));
  assert(CountSelectedQtEdges(q) == 1);
  q.Undo();
  QVERIFY(CountSelectedQtEdges(q) == 0);
}

void ribi::cmap::QtCommandSelectTest::SelectQtEdgeConnectedToCenter() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(CountSelectedQtEdges(q) == 0);
  q.DoCommand(new CommandSelect(q, *GetFirstQtEdge(q)));
  QVERIFY(CountSelectedQtEdges(q) == 1);
}

void ribi::cmap::QtCommandSelectTest::SelectQtNode() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(CountSelectedQtNodes(q) == 0);
  q.DoCommand(new CommandSelect(q, *FindFirstQtNode(q, QtNodeHasName("one"))));
  QVERIFY(CountSelectedQtNodes(q) == 1);
}

void ribi::cmap::QtCommandSelectTest::SelectQtNodeAndUndo() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(CountSelectedQtNodes(q) == 0);
  q.DoCommand(new CommandSelect(q, *FindFirstQtNode(q, QtNodeHasName("one"))));
  assert(CountSelectedQtNodes(q) == 1);
  q.Undo();
  QVERIFY(CountSelectedQtNodes(q) == 0);
}

void ribi::cmap::QtCommandSelectTest::ParseCenterNode() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  const auto c = ParseCommandSelect(q, "select(center)");
  QVERIFY(c != nullptr);
}

void ribi::cmap::QtCommandSelectTest::ParseEdge() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  const auto c = ParseCommandSelect(q, "select(first)");
  QVERIFY(c != nullptr);
}

void ribi::cmap::QtCommandSelectTest::ParseNode() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  const auto c = ParseCommandSelect(q, "select(one)");
  QVERIFY(c != nullptr);
}

void ribi::cmap::QtCommandSelectTest::ParseNonsenseFails() const noexcept
{
  QtConceptMap q;
  QVERIFY(ParseCommandSelect(q, "nonsense") == nullptr);
}
