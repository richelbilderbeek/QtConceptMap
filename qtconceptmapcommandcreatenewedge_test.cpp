#include "qtconceptmapcommandcreatenewedge_test.h"

#include "qtconceptmapcommandcreatenewedge.h"
#include "qtconceptmapcommandcreatenewnode.h"
#include "qtconceptmapcommandselectnode.h"
#include "qtconceptmap.h"

#include <QDebug>

void ribi::cmap::QtConceptMapCommandCreateNewEdgeTest::CreateNewEdge() const noexcept
{
  QtConceptMap q;
  q.DoCommand(new CommandCreateNewNode(q, "from", NodeType::normal, -100, -200));
  q.DoCommand(new CommandCreateNewNode(q, "to"  , NodeType::normal,  300,  400));
  QVERIFY(CountQtNodes(q) == 2);
  QVERIFY(CountQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 2);
  QVERIFY(CountSelectedQtEdges(q) == 0);
  q.DoCommand(new CommandCreateNewEdge(q, "between"));
  QVERIFY(CountQtNodes(q) == 2);
  QVERIFY(CountQtEdges(q) == 1);
  QVERIFY(CountSelectedQtNodes(q) == 0);
  QVERIFY(CountSelectedQtEdges(q) == 1);
  QVERIFY(!GetQtToolItemBuddy(q));
}

void ribi::cmap::QtConceptMapCommandCreateNewEdgeTest::CreateNewEdgeUndo() const noexcept
{
  QtConceptMap q;
  q.DoCommand(new CommandCreateNewNode(q, "from", NodeType::normal, -100, -200));
  q.DoCommand(new CommandCreateNewNode(q, "to"  , NodeType::normal,  300,  400));
  assert(CountQtNodes(q) == 2);
  assert(CountQtEdges(q) == 0);
  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 2);
  q.DoCommand(new CommandCreateNewEdge(q, "between"));
  assert(CountQtNodes(q) == 2);
  assert(CountQtEdges(q) == 1);
  QVERIFY(CountSelectedQtEdges(q) == 1);
  QVERIFY(CountSelectedQtNodes(q) == 0);
  q.Undo();
  assert(CountQtNodes(q) == 2);
  assert(CountQtEdges(q) == 0);
  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 2);
}

void ribi::cmap::QtConceptMapCommandCreateNewEdgeTest::CreateNewEdgeFromCenterNode() const noexcept
{
  QtConceptMap q;
  q.DoCommand(new CommandCreateNewNode(q, "center"   , NodeType::center , -100, -200));
  q.DoCommand(new CommandCreateNewNode(q, "something", NodeType::normal,  300,  400));
  assert(CountQtNodes(q) == 2);
  assert(CountQtEdges(q) == 0);
  q.DoCommand(new CommandCreateNewEdge(q, "invisible"));
  QVERIFY(CountQtNodes(q) == 2);
  QVERIFY(CountQtEdges(q) == 1);
  QVERIFY(CountSelectedQtEdges(q) == 1);
  QVERIFY(CountSelectedQtNodes(q) == 0);
}

void ribi::cmap::QtConceptMapCommandCreateNewEdgeTest::Parse() const noexcept
{
  QtConceptMap q;
  const auto c1 = ParseCommandCreateNewNode(q, "create_new_node(from, 10, 20, normal)");
  q.DoCommand(c1);
  assert(CountSelectedQtNodes(q) == 1);
  const auto c2 = ParseCommandCreateNewNode(q, "create_new_node(to, 10, 40, normal)");
  q.DoCommand(c2);
  assert(CountSelectedQtNodes(q) == 2);
  const auto c3 = ParseCommandCreateNewEdge(q, "create_new_edge(my text)");
  QVERIFY(c3 != nullptr);
  QVERIFY(GetText(*c3) == "my text");
  q.DoCommand(c3);
  QVERIFY(CountSelectedQtEdges(q) == 1);
  QVERIFY(CountSelectedQtNodes(q) == 0);
}

void ribi::cmap::QtConceptMapCommandCreateNewEdgeTest::ParseNonsenseFails() const noexcept
{
  QtConceptMap q;
  QVERIFY(ParseCommandCreateNewEdge(q, "nonsense") == nullptr);
}
