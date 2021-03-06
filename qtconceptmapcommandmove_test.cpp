#include "qtconceptmapcommandmove_test.h"

#include "qtconceptmapcommandmove.h"
#include "qtconceptmap.h"
#include "conceptmapfactory.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmaphelper.h"


void ribi::cmap::QtCommandMoveTest::MoveAbsentItemByNameFails() const noexcept
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

void ribi::cmap::QtCommandMoveTest::MoveCenterQtNodeByNameFails() const noexcept
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

void ribi::cmap::QtCommandMoveTest::MoveOneOfTwoQtEdgesByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetThreeNodeTwoEdge());

  assert(GetX(*FindFirstQtEdge(q, QtEdgeHasName("second"))) == 350);
  assert(GetY(*FindFirstQtEdge(q, QtEdgeHasName("second"))) == 275);
  q.DoCommand(new CommandMove(q, "second", 25, 125));
  assert(q.GetUndo().count() == 1);
  assert(q.GetUndo().command(0));
  assert( dynamic_cast<const CommandMove*>(q.GetUndo().command(0)));
  assert(!dynamic_cast<const CommandMove*>(q.GetUndo().command(0))->GetMovedQtNode());
  assert( dynamic_cast<const CommandMove*>(q.GetUndo().command(0))->GetMovedQtEdge());
  assert(GetX(*FindFirstQtEdge(q, QtEdgeHasName("second"))) == 375);
  QVERIFY(GetX(*FindFirstQtEdge(q, QtEdgeHasName("second"))) == 375);
  QVERIFY(GetY(*FindFirstQtEdge(q, QtEdgeHasName("second"))) == 400);
}

void ribi::cmap::QtCommandMoveTest::MoveOneOfTwoQtEdgesConnectedToCenterQtNodeByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetThreeNodeTwoEdge());
  assert(GetX(*FindFirstQtEdge(q, QtEdgeHasName("first"))) == 150);
  assert(GetY(*FindFirstQtEdge(q, QtEdgeHasName("first"))) == 225);
  q.DoCommand(new CommandMove(q, "first", 25, 125));
  assert(q.GetUndo().count() == 1);
  assert(q.GetUndo().command(0));
  assert( dynamic_cast<const CommandMove*>(q.GetUndo().command(0)));
  assert(!dynamic_cast<const CommandMove*>(q.GetUndo().command(0))->GetMovedQtNode());
  assert( dynamic_cast<const CommandMove*>(q.GetUndo().command(0))->GetMovedQtEdge());
  assert(GetX(*FindFirstQtEdge(q, QtEdgeHasName("first"))) == 175);
  QVERIFY(GetX(*FindFirstQtEdge(q, QtEdgeHasName("first"))) == 175);
  QVERIFY(GetY(*FindFirstQtEdge(q, QtEdgeHasName("first"))) == 350);
}


void ribi::cmap::QtCommandMoveTest::MoveOnlyQtEdgeByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdgeNoCenter());
  assert(GetX(*FindFirstQtEdge(q, QtEdgeHasName("second"))) == 350);
  assert(GetY(*FindFirstQtEdge(q, QtEdgeHasName("second"))) == 275);
  q.DoCommand(new CommandMove(q, "second", 25, 125));
  assert(q.GetUndo().count() == 1);
  assert(q.GetUndo().command(0));
  assert( dynamic_cast<const CommandMove*>(q.GetUndo().command(0)));
  assert(!dynamic_cast<const CommandMove*>(q.GetUndo().command(0))->GetMovedQtNode());
  assert( dynamic_cast<const CommandMove*>(q.GetUndo().command(0))->GetMovedQtEdge());
  const QtEdge * const qtedge = dynamic_cast<const CommandMove*>(q.GetUndo().command(0))->GetMovedQtEdge();

  assert(q.GetScene().items().contains(const_cast<QtEdge*>(qtedge)));

  //One edge
  assert(CountQtEdges(q) == 1);
  assert(boost::num_edges(q.ToConceptMap()) == 1);
  assert(GetQtEdges(q).size() == 1);

  assert(GetQtEdges(q)[0] == qtedge);

  const double expected_x{375.0};
  const double expected_y{400.0};
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

void ribi::cmap::QtCommandMoveTest::MoveOnlyQtEdgeConnectedToCenterQtNode() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(GetX(*FindFirstQtEdge(q, QtEdgeHasName("first"))) == 150);
  assert(GetY(*FindFirstQtEdge(q, QtEdgeHasName("first"))) == 225);
  q.DoCommand(new CommandMove(q, "first", 25, 125));
  assert(q.GetUndo().count() == 1);
  assert(q.GetUndo().command(0));
  assert( dynamic_cast<const CommandMove*>(q.GetUndo().command(0)));
  assert(!dynamic_cast<const CommandMove*>(q.GetUndo().command(0))->GetMovedQtNode());
  assert( dynamic_cast<const CommandMove*>(q.GetUndo().command(0))->GetMovedQtEdge());
  assert(GetX(*FindFirstQtEdge(q, QtEdgeHasName("first"))) == 175);
  QVERIFY(GetX(*FindFirstQtEdge(q, QtEdgeHasName("first"))) == 175);
  QVERIFY(GetY(*FindFirstQtEdge(q, QtEdgeHasName("first"))) == 350);
}

void ribi::cmap::QtCommandMoveTest::MoveQtNodeByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetThreeNodeTwoEdge());
  assert(GetX(*FindFirstQtNode(q, QtNodeHasName("one"))) == 300);
  assert(GetY(*FindFirstQtNode(q, QtNodeHasName("one"))) == 250);
  q.DoCommand(new CommandMove(q, "one", 25, 125));
  QVERIFY(GetX(*FindFirstQtNode(q, QtNodeHasName("one"))) == 325);
  QVERIFY(GetY(*FindFirstQtNode(q, QtNodeHasName("one"))) == 375);
}

void ribi::cmap::QtCommandMoveTest::MoveQtNodeByNameAndUndo() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetThreeNodeTwoEdge());
  assert(GetX(*FindFirstQtNode(q, QtNodeHasName("one"))) == 300);
  assert(GetY(*FindFirstQtNode(q, QtNodeHasName("one"))) == 250);
  q.DoCommand(new CommandMove(q, "one", 25, 125));
  assert(GetX(*FindFirstQtNode(q, QtNodeHasName("one"))) != 300);
  assert(GetY(*FindFirstQtNode(q, QtNodeHasName("one"))) != 250);
  q.Undo();
  QVERIFY(GetX(*FindFirstQtNode(q, QtNodeHasName("one"))) == 300);
  QVERIFY(GetY(*FindFirstQtNode(q, QtNodeHasName("one"))) == 250);
}

void ribi::cmap::QtCommandMoveTest::Parse() const noexcept
{
  QtConceptMap q;
  const auto c = ParseCommandMove(q, "move(node label, 10, 20)");
  QVERIFY(c != nullptr);
  QVERIFY(c->GetName() == "node label");
  QVERIFY(c->GetDx() == 10.0);
  QVERIFY(c->GetDy() == 20.0);
}

void ribi::cmap::QtCommandMoveTest::ParseNonsenseFails() const noexcept
{
  QtConceptMap q;
  QVERIFY(ParseCommandMove(q, "nonsense") == nullptr);
  QVERIFY(ParseCommandMove(q, "move(nonsense") == nullptr);
  QVERIFY(ParseCommandMove(q, "move(nonsense)") == nullptr);
  QVERIFY(ParseCommandMove(q, "move(non, sense)") == nullptr);
}
