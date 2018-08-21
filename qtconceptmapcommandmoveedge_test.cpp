#include "qtconceptmapcommandmoveedge_test.h"

#include "qtconceptmapcommandmoveedge.h"
#include "qtconceptmap.h"
#include "conceptmapfactory.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmaphelper.h"

void ribi::cmap::QtCommandMoveEdgeTest::MoveAbsentItemByNameFails() const noexcept
{
  QtConceptMap q;
  const auto concept_map = ConceptMapFactory().GetThreeNodeTwoEdge();
  q.SetConceptMap(concept_map);
  try
  {
    QtEdge * const first_qtedge = FindFirstQtEdge(
      q,
      [](const QtEdge * const qtedge) { return GetText(*qtedge) == "absent"; }
    );
    q.DoCommand(new CommandMoveEdge(q, first_qtedge, 25, 125));
    QVERIFY(!"Should not get here");
  }
  catch (std::exception&)
  {
    QVERIFY("OK");
  }
}

void ribi::cmap::QtCommandMoveEdgeTest::MoveOneOfTwoQtEdgesByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetThreeNodeTwoEdge());
  assert(GetX(*FindFirstQtEdge(q, [](QtEdge * qtedge) { return GetText(*qtedge) == "second"; })) == 350);
  assert(GetY(*FindFirstQtEdge(q, [](QtEdge * qtedge) { return GetText(*qtedge) == "second"; })) == 275);
  QtEdge * const first_qtedge = FindFirstQtEdge(
    q,
    [](const QtEdge * const qtedge) { return GetText(*qtedge) == "second"; }
  );
  q.DoCommand(new CommandMoveEdge(q, first_qtedge, 25, 125));
  assert(q.GetUndo().count() == 1);
  assert(q.GetUndo().command(0));
  assert( dynamic_cast<const CommandMoveEdge*>(q.GetUndo().command(0)));
  assert( dynamic_cast<const CommandMoveEdge*>(q.GetUndo().command(0))->GetMovedQtEdge());
  assert(GetX(*FindFirstQtEdge(q, [](QtEdge * qtedge) { return GetText(*qtedge) == "second"; })) == 375);
  QVERIFY(GetX(*FindFirstQtEdge(q, [](QtEdge * qtedge) { return GetText(*qtedge) == "second"; })) == 375);
  QVERIFY(GetY(*FindFirstQtEdge(q, [](QtEdge * qtedge) { return GetText(*qtedge) == "second"; })) == 400);
}

void ribi::cmap::QtCommandMoveEdgeTest::MoveOneOfTwoQtEdgesConnectedToCenterQtNodeByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetThreeNodeTwoEdge());
  assert(GetX(*FindFirstQtEdge(q, [](QtEdge * qtedge) { return GetText(*qtedge) == "first"; })) == 150);
  assert(GetY(*FindFirstQtEdge(q, [](QtEdge * qtedge) { return GetText(*qtedge) == "first"; })) == 225);
  QtEdge * const first_qtedge = FindFirstQtEdge(
    q,
    [](const QtEdge * const qtedge) { return GetText(*qtedge) == "first"; }
  );
  q.DoCommand(new CommandMoveEdge(q, first_qtedge, 25, 125));
  assert(q.GetUndo().count() == 1);
  assert(q.GetUndo().command(0));
  assert( dynamic_cast<const CommandMoveEdge*>(q.GetUndo().command(0)));
  assert( dynamic_cast<const CommandMoveEdge*>(q.GetUndo().command(0))->GetMovedQtEdge());
  assert(GetX(*FindFirstQtEdge(q, [](QtEdge * qtedge) { return GetText(*qtedge) == "first"; })) == 175);
  QVERIFY(GetX(*FindFirstQtEdge(q, [](QtEdge * qtedge) { return GetText(*qtedge) == "first"; })) == 175);
  QVERIFY(GetY(*FindFirstQtEdge(q, [](QtEdge * qtedge) { return GetText(*qtedge) == "first"; })) == 350);
}


void ribi::cmap::QtCommandMoveEdgeTest::MoveOnlyQtEdgeByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdgeNoCenter());
  QtEdge * const qtedge = GetFirstQtEdge(q);
  const double x{GetX(*qtedge)};
  const double y{GetY(*qtedge)};
  const double dx{25.0};
  const double dy{125.0};
  q.DoCommand(new CommandMoveEdge(q, qtedge, dx, dy));
  const double expected_x{x + dx};
  const double expected_y{y + dy};
  const double measured_x1{GetX(*qtedge)};
  const double measured_y1{GetY(*qtedge)};
  const double measured_x3{GetX(*qtedge->GetQtNode())};
  const double measured_y3{GetY(*qtedge->GetQtNode())};
  const double measured_x4{GetX(GetFirstEdge(q.ToConceptMap()))};
  const double measured_y4{GetY(GetFirstEdge(q.ToConceptMap()))};
  QVERIFY(std::abs(expected_x - measured_x1) < 2.0);
  QVERIFY(std::abs(expected_y - measured_y1) < 2.0);
  QVERIFY(std::abs(expected_x - measured_x3) < 2.0);
  QVERIFY(std::abs(expected_y - measured_y3) < 2.0);
  QVERIFY(std::abs(expected_x - measured_x4) < 2.0);
  QVERIFY(std::abs(expected_y - measured_y4) < 2.0);
}

void ribi::cmap::QtCommandMoveEdgeTest::MoveOnlyQtEdgeByNameAndUndo() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdgeNoCenter());
  QtEdge * const qtedge = GetFirstQtEdge(q);
  const double x{GetX(*qtedge)};
  const double y{GetY(*qtedge)};
  const double dx{25.0};
  const double dy{125.0};
  q.DoCommand(new CommandMoveEdge(q, qtedge, dx, dy));
  assert(std::abs(GetX(*qtedge) - x) > 10.0);
  assert(std::abs(GetY(*qtedge) - y) > 10.0);
  q.Undo();
  QVERIFY(std::abs(GetX(*qtedge) - x) < 2.0);
  QVERIFY(std::abs(GetY(*qtedge) - y) < 2.0);
}

void ribi::cmap::QtCommandMoveEdgeTest::MoveOnlyQtEdgeConnectedToCenterQtNode() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(GetX(*FindFirstQtEdge(q, [](QtEdge * qtedge) { return GetText(*qtedge) == "first"; })) == 150);
  assert(GetY(*FindFirstQtEdge(q, [](QtEdge * qtedge) { return GetText(*qtedge) == "first"; })) == 225);
  QtEdge * const first_qtedge = FindFirstQtEdge(
    q,
    [](const QtEdge * const qtedge) { return GetText(*qtedge) == "first"; }
  );
  q.DoCommand(new CommandMoveEdge(q, first_qtedge, 25, 125));
  assert(q.GetUndo().count() == 1);
  assert(q.GetUndo().command(0));
  assert( dynamic_cast<const CommandMoveEdge*>(q.GetUndo().command(0)));
  assert( dynamic_cast<const CommandMoveEdge*>(q.GetUndo().command(0))->GetMovedQtEdge());
  assert(GetX(*FindFirstQtEdge(q, [](QtEdge * qtedge) { return GetText(*qtedge) == "first"; })) == 175);
  QVERIFY(GetX(*FindFirstQtEdge(q, [](QtEdge * qtedge) { return GetText(*qtedge) == "first"; })) == 175);
  QVERIFY(GetY(*FindFirstQtEdge(q, [](QtEdge * qtedge) { return GetText(*qtedge) == "first"; })) == 350);
}

void ribi::cmap::QtCommandMoveEdgeTest::Parse() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  const auto c = ParseCommandMoveEdge(q, "move_edge(first, 10, 20)");
  assert(c);
  QVERIFY(c != nullptr);
  QVERIFY(c->GetDx() == 10.0);
  QVERIFY(c->GetDy() == 20.0);
}

void ribi::cmap::QtCommandMoveEdgeTest::ParseNonsenseFails() const noexcept
{
  QtConceptMap q;
  QVERIFY(ParseCommandMoveEdge(q, "nonsense") == nullptr);
}
