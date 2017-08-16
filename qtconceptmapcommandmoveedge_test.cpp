#include "qtconceptmapcommandmoveedge_test.h"

#include "qtconceptmapcommandmoveedge.h"
#include "qtconceptmap.h"
#include "conceptmapfactory.h"


void ribi::cmap::QtConceptMapCommandMoveEdgeTest::MoveAbsentItemByNameFails() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetThreeNodeTwoEdge());
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

void ribi::cmap::QtConceptMapCommandMoveEdgeTest::MoveOneOfTwoQtEdgesByName() const noexcept
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

void ribi::cmap::QtConceptMapCommandMoveEdgeTest::MoveOneOfTwoQtEdgesConnectedToCenterQtNodeByName() const noexcept
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


void ribi::cmap::QtConceptMapCommandMoveEdgeTest::MoveOnlyQtEdgeByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdgeNoCenter());
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
  const QtEdge * const qtedge = dynamic_cast<const CommandMoveEdge*>(q.GetUndo().command(0))->GetMovedQtEdge();

  assert(q.GetScene().items().contains(const_cast<QtEdge*>(qtedge)));

  //One edge
  assert(CountQtEdges(q) == 1);
  assert(boost::num_edges(q.GetConceptMap()) == 1);
  assert(GetQtEdges(q).size() == 1);

  assert(GetQtEdges(q)[0] == qtedge);
  assert(HasSameData(qtedge->GetEdge(), GetFirstEdge(q.GetConceptMap())));
  assert(HasSameData(GetQtEdges(q)[0]->GetEdge(), GetFirstEdge(q.GetConceptMap())));
  assert(GetQtEdges(q)[0]->GetEdge() == GetFirstEdge(q.GetConceptMap()));

  const double expected_x{375.0};
  const double expected_y{400.0};
  const double measured_x1{GetX(*qtedge)};
  const double measured_y1{GetY(*qtedge)};
  const double measured_x2{GetX(qtedge->GetEdge())};
  const double measured_y2{GetY(qtedge->GetEdge())};
  const double measured_x3{GetX(*qtedge->GetQtNode())};
  const double measured_y3{GetY(*qtedge->GetQtNode())};
  const double measured_x4{GetX(GetFirstEdge(q.GetConceptMap()))};
  const double measured_y4{GetY(GetFirstEdge(q.GetConceptMap()))};

  QVERIFY(std::abs(expected_x - measured_x1) < 2.0);
  QVERIFY(std::abs(expected_y - measured_y1) < 2.0);
  QVERIFY(std::abs(expected_x - measured_x2) < 2.0);
  QVERIFY(std::abs(expected_y - measured_y2) < 2.0);
  QVERIFY(std::abs(expected_x - measured_x3) < 2.0);
  QVERIFY(std::abs(expected_y - measured_y3) < 2.0);
  QVERIFY(std::abs(expected_x - measured_x4) < 2.0);
  QVERIFY(std::abs(expected_y - measured_y4) < 2.0);
}

void ribi::cmap::QtConceptMapCommandMoveEdgeTest::MoveOnlyQtEdgeConnectedToCenterQtNode() const noexcept
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

void ribi::cmap::QtConceptMapCommandMoveEdgeTest::Parse() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  const auto c = ParseCommandMoveEdge(q, "move_edge(one, 10, 20)");
  QVERIFY(c != nullptr);
  QVERIFY(c->GetDx() == 10.0);
  QVERIFY(c->GetDy() == 20.0);
}

void ribi::cmap::QtConceptMapCommandMoveEdgeTest::ParseNonsenseFails() const noexcept
{
  QtConceptMap q;
  QVERIFY(ParseCommandMoveEdge(q, "nonsense") == nullptr);
}
