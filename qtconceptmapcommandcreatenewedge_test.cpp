#include "qtconceptmapcommandcreatenewedge_test.h"

#include "qtconceptmapcommandcreatenewedge.h"
#include "qtconceptmapcommandcreatenewnode.h"
#include "qtconceptmap.h"

#include <QDebug>

void ribi::cmap::QtConceptMapCommandCreateNewEdgeTest::CreateNewEdge() const noexcept
{
  QtConceptMap q;
  q.DoCommand(new CommandCreateNewNode(q, "from", false, -100, -200));
  q.DoCommand(new CommandCreateNewNode(q, "to"  , false,  300,  400));
  assert(CountQtNodes(q) == 2);
  assert(CountQtEdges(q) == 0);
  q.DoCommand(new CommandCreateNewEdgeBetweenTwoSelectedNodes(q, "between"));
  QVERIFY(CountQtNodes(q) == 2);
  QVERIFY(CountQtEdges(q) == 1);
  QVERIFY(!GetQtExamplesItemBuddy(q));
  QVERIFY(!GetQtToolItemBuddy(q));
}

void ribi::cmap::QtConceptMapCommandCreateNewEdgeTest::CreateNewEdgeUndo() const noexcept
{
  QtConceptMap q;
  q.DoCommand(new CommandCreateNewNode(q, "from", false, -100, -200));
  q.DoCommand(new CommandCreateNewNode(q, "to"  , false,  300,  400));
  assert(CountQtNodes(q) == 2);
  assert(CountQtEdges(q) == 0);
  q.DoCommand(new CommandCreateNewEdgeBetweenTwoSelectedNodes(q, "between"));
  assert(CountQtNodes(q) == 2);
  assert(CountQtEdges(q) == 1);
  q.Undo();
  assert(CountQtNodes(q) == 2);
  assert(CountQtEdges(q) == 0);
}

void ribi::cmap::QtConceptMapCommandCreateNewEdgeTest::CreateNewEdgeFromCenterNode() const noexcept
{
  QtConceptMap q;
  q.DoCommand(new CommandCreateNewNode(q, "center"   , true , -100, -200));
  q.DoCommand(new CommandCreateNewNode(q, "something", false,  300,  400));
  assert(CountQtNodes(q) == 2);
  assert(CountQtEdges(q) == 0);
  q.DoCommand(new CommandCreateNewEdgeBetweenTwoSelectedNodes(q, "invisible"));
  QVERIFY(CountQtNodes(q) == 2);
  QVERIFY(CountQtEdges(q) == 1);
}

void ribi::cmap::QtConceptMapCommandCreateNewEdgeTest::Parse() const noexcept
{
  QtConceptMap q;
  const auto c1 = ParseCommandCreateNewNode(q, "create_new_node(my text, false, 10, 20)");
  q.DoCommand(c1);
  assert(CountSelectedQtNodes(q) == 1);
  const auto c2 = ParseCommandCreateNewNode(q, "create_new_node(my text, false, 10, 40)");
  q.DoCommand(c2);
  assert(CountSelectedQtNodes(q) == 2);
  const auto c3 = ParseCommandCreateNewEdge(q, "create_new_edge(my text)");
  QVERIFY(c3 != nullptr);
  QVERIFY(GetText(*c3) == "my text");
  q.DoCommand(c3);
}

void ribi::cmap::QtConceptMapCommandCreateNewEdgeTest::ParseNonsenseFails() const noexcept
{
  QtConceptMap q;
  QVERIFY(ParseCommandCreateNewEdge(q, "nonsense") == nullptr);
}
