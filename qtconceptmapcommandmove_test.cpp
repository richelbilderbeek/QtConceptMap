#include "qtconceptmapcommandmove_test.h"

#include "qtconceptmapcommandmove.h"
#include "qtconceptmap.h"
#include "conceptmapfactory.h"


void ribi::cmap::QtConceptMapCommandMoveTest::MoveAbsentItemFails() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetThreeNodeTwoEdge());
  try
  {
    q.DoCommand(new CommandMove(q, "absent", 25, 125));
    QVERIFY(!"Should not get here");
  }
  catch (std::exception&)
  {
    QVERIFY("OK");
  }
}

void ribi::cmap::QtConceptMapCommandMoveTest::MoveCenterQtNodeFails() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetThreeNodeTwoEdge());
  try
  {
    q.DoCommand(new CommandMove(q, "center", 25, 125));
    QVERIFY(!"Should not get here");
  }
  catch (std::exception&)
  {
    QVERIFY("OK");
  }
}

void ribi::cmap::QtConceptMapCommandMoveTest::MoveQtEdge() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetThreeNodeTwoEdge());
  assert(GetX(*FindFirstQtEdge(q, [](QtEdge * qtedge) { return GetText(*qtedge) == "second"; })) == 350);
  assert(GetY(*FindFirstQtEdge(q, [](QtEdge * qtedge) { return GetText(*qtedge) == "second"; })) == 275);
  q.DoCommand(new CommandMove(q, "second", 25, 125));
  assert(q.GetUndo().count() == 1);
  assert(q.GetUndo().command(0));
  assert( dynamic_cast<const CommandMove*>(q.GetUndo().command(0)));
  assert(!dynamic_cast<const CommandMove*>(q.GetUndo().command(0))->GetMovedQtNode());
  assert( dynamic_cast<const CommandMove*>(q.GetUndo().command(0))->GetMovedQtEdge());
  assert(GetX(*FindFirstQtEdge(q, [](QtEdge * qtedge) { return GetText(*qtedge) == "second"; })) == 375);
  QVERIFY(GetX(*FindFirstQtEdge(q, [](QtEdge * qtedge) { return GetText(*qtedge) == "second"; })) == 375);
  QVERIFY(GetY(*FindFirstQtEdge(q, [](QtEdge * qtedge) { return GetText(*qtedge) == "second"; })) == 400);
}

void ribi::cmap::QtConceptMapCommandMoveTest::MoveQtEdgeConnectedToCenterQtNode() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetThreeNodeTwoEdge());
  assert(GetX(*FindFirstQtEdge(q, [](QtEdge * qtedge) { return GetText(*qtedge) == "first"; })) == 150);
  assert(GetY(*FindFirstQtEdge(q, [](QtEdge * qtedge) { return GetText(*qtedge) == "first"; })) == 225);
  q.DoCommand(new CommandMove(q, "first", 25, 125));
  assert(q.GetUndo().count() == 1);
  assert(q.GetUndo().command(0));
  assert( dynamic_cast<const CommandMove*>(q.GetUndo().command(0)));
  assert(!dynamic_cast<const CommandMove*>(q.GetUndo().command(0))->GetMovedQtNode());
  assert( dynamic_cast<const CommandMove*>(q.GetUndo().command(0))->GetMovedQtEdge());
  assert(GetX(*FindFirstQtEdge(q, [](QtEdge * qtedge) { return GetText(*qtedge) == "first"; })) == 175);
  QVERIFY(GetX(*FindFirstQtEdge(q, [](QtEdge * qtedge) { return GetText(*qtedge) == "first"; })) == 175);
  QVERIFY(GetY(*FindFirstQtEdge(q, [](QtEdge * qtedge) { return GetText(*qtedge) == "first"; })) == 350);
}


void ribi::cmap::QtConceptMapCommandMoveTest::MoveQtNode() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetThreeNodeTwoEdge());
  assert(GetX(*FindFirstQtNode(q, [](QtNode * qtnode) { return GetText(*qtnode) == "one"; })) == 300);
  assert(GetY(*FindFirstQtNode(q, [](QtNode * qtnode) { return GetText(*qtnode) == "one"; })) == 250);
  q.DoCommand(new CommandMove(q, "one", 25, 125));
  QVERIFY(GetX(*FindFirstQtNode(q, [](QtNode * qtnode) { return GetText(*qtnode) == "one"; })) == 325);
  QVERIFY(GetY(*FindFirstQtNode(q, [](QtNode * qtnode) { return GetText(*qtnode) == "one"; })) == 375);
}

void ribi::cmap::QtConceptMapCommandMoveTest::Parse() const noexcept
{
  QtConceptMap q;
  const auto c = ParseCommandMove(q, "move(node label, 10, 20)");
  QVERIFY(c != nullptr);
  QVERIFY(c->GetName() == "node label");
  QVERIFY(c->GetDx() == 10.0);
  QVERIFY(c->GetDy() == 20.0);
}

void ribi::cmap::QtConceptMapCommandMoveTest::ParseNonsenseFails() const noexcept
{
  QtConceptMap q;
  QVERIFY(ParseCommandMove(q, "nonsense") == nullptr);
}
