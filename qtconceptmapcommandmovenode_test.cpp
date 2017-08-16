#include "qtconceptmapcommandmovenode_test.h"

#include "qtconceptmapcommandmovenode.h"
#include "qtconceptmap.h"
#include "conceptmapfactory.h"


void ribi::cmap::QtConceptMapCommandMoveNodeTest::MoveAbsentItemByNameFails() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetThreeNodeTwoEdge());
  try
  {
    q.DoCommand(new CommandMoveNode(q, QtNodeHasText("absent"), 25, 125));
    QVERIFY(!"Should not get here");
  }
  catch (std::exception&)
  {
    QVERIFY("OK");
  }
}

void ribi::cmap::QtConceptMapCommandMoveNodeTest::MoveCenterQtNodeByNameFails() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetThreeNodeTwoEdge());
  try
  {
    q.DoCommand(new CommandMoveNode(q, QtNodeHasText("center"), 25, 125));
    QVERIFY(!"Should not get here");
  }
  catch (std::exception&)
  {
    QVERIFY("OK");
  }
}

void ribi::cmap::QtConceptMapCommandMoveNodeTest::MoveEdgeFails() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetThreeNodeTwoEdge());
  assert(GetX(*FindFirstQtEdge(q, [](QtEdge * qtedge) { return GetText(*qtedge) == "second"; })) == 350);
  assert(GetY(*FindFirstQtEdge(q, [](QtEdge * qtedge) { return GetText(*qtedge) == "second"; })) == 275);
  try
  {
    q.DoCommand(new CommandMoveNode(q, QtNodeHasText("second"), 25, 125));
    QVERIFY(!"Should not get here"); //!OCLINT accepted idiom
  }
  catch (std::exception&)
  {
    QVERIFY("OK"); //!OCLINT accepted idiom
  }
}

void ribi::cmap::QtConceptMapCommandMoveNodeTest::MoveEdgeConnectedToCenterFails() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetThreeNodeTwoEdge());
  assert(GetX(*FindFirstQtEdge(q, [](QtEdge * qtedge) { return GetText(*qtedge) == "first"; })) == 150);
  assert(GetY(*FindFirstQtEdge(q, [](QtEdge * qtedge) { return GetText(*qtedge) == "first"; })) == 225);
  try
  {
    q.DoCommand(new CommandMoveNode(q, QtNodeHasText("first"), 25, 125));
    QVERIFY(!"Should not get here"); //!OCLINT accepted idiom
  }
  catch (std::exception&)
  {
    QVERIFY("OK"); //!OCLINT accepted idiom
  }
}

void ribi::cmap::QtConceptMapCommandMoveNodeTest::MoveQtNodeWithName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetThreeNodeTwoEdge());
  assert(GetX(*FindFirstQtNode(q, [](QtNode * qtnode) { return GetText(*qtnode) == "one"; })) == 300);
  assert(GetY(*FindFirstQtNode(q, [](QtNode * qtnode) { return GetText(*qtnode) == "one"; })) == 250);
  q.DoCommand(new CommandMoveNode(q, QtNodeHasText("one"), 25, 125));
  QVERIFY(GetX(*FindFirstQtNode(q, [](QtNode * qtnode) { return GetText(*qtnode) == "one"; })) == 325);
  QVERIFY(GetY(*FindFirstQtNode(q, [](QtNode * qtnode) { return GetText(*qtnode) == "one"; })) == 375);
}

void ribi::cmap::QtConceptMapCommandMoveNodeTest
  ::MoveQtNodeWithPosition() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetThreeNodeTwoEdge());
  assert(GetX(*FindFirstQtNode(q, [](QtNode * qtnode) { return GetText(*qtnode) == "one"; })) == 300);
  assert(GetY(*FindFirstQtNode(q, [](QtNode * qtnode) { return GetText(*qtnode) == "one"; })) == 250);

  q.DoCommand(
    new CommandMoveNode(
      q,
      [](const QtNode& qtnode) { return GetX(qtnode) == 300 && GetY(qtnode) == 250; },
      25,
      125
    )
  );
  QVERIFY(GetX(*FindFirstQtNode(q, [](QtNode * qtnode) { return GetText(*qtnode) == "one"; })) == 325);
  QVERIFY(GetY(*FindFirstQtNode(q, [](QtNode * qtnode) { return GetText(*qtnode) == "one"; })) == 375);
}

void ribi::cmap::QtConceptMapCommandMoveNodeTest::Parse() const noexcept
{
  QtConceptMap q;
  const auto c = ParseCommandMoveNode(q, "move_node(node label, 10, 20)");
  QVERIFY(c != nullptr);
  QVERIFY(c->GetDx() == 10.0);
  QVERIFY(c->GetDy() == 20.0);
}

void ribi::cmap::QtConceptMapCommandMoveNodeTest::ParseNonsenseFails() const noexcept
{
  QtConceptMap q;
  QVERIFY(ParseCommandMoveNode(q, "nonsense") == nullptr);
}