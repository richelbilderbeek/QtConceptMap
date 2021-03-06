#include "qtconceptmapcommandmovenode_test.h"

#include "qtconceptmapcommandmovenode.h"
#include "qtconceptmap.h"
#include "conceptmapfactory.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmapqtnode.h"

void ribi::cmap::QtCommandMoveNodeTest::MoveAbsentItemByNameFails() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetThreeNodeTwoEdge());
  try
  {
    QtNode * const first_qtnode = FindFirstQtNode(
      q,
      [name = "absent", &q](QtNode * const qtnode)
      {
        return name == GetText(*qtnode);
      }
    );
    q.DoCommand(new CommandMoveNode(q, first_qtnode, 25, 125));
    QVERIFY(!"Should not get here");
  }
  catch (std::exception&)
  {
    QVERIFY("OK");
  }
}

void ribi::cmap::QtCommandMoveNodeTest::MoveCenterQtNodeByNameFails() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetThreeNodeTwoEdge());
  try
  {
    QtNode * const first_qtnode = FindFirstQtNode(
      q,
      [name = "center", &q](QtNode * const qtnode)
      {
        return name == GetText(*qtnode);
      }
    );
    q.DoCommand(new CommandMoveNode(q, first_qtnode, 25, 125));
    QVERIFY(!"Should not get here");
  }
  catch (std::exception&)
  {
    QVERIFY("OK");
  }
}

void ribi::cmap::QtCommandMoveNodeTest::MoveEdgeFails() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetThreeNodeTwoEdge());
  assert(GetX(*FindFirstQtEdge(q, QtEdgeHasName("second"))) == 350);
  assert(GetY(*FindFirstQtEdge(q, QtEdgeHasName("second"))) == 275);
  try
  {
    QtNode * const first_qtnode = FindFirstQtNode(
      q,
      [name = "second", &q](QtNode * const qtnode)
      {
        return name == GetText(*qtnode);
      }
    );
    q.DoCommand(new CommandMoveNode(q, first_qtnode, 25, 125));
    QVERIFY(!"Should not get here"); //!OCLINT accepted idiom
  }
  catch (std::exception&)
  {
    QVERIFY("OK"); //!OCLINT accepted idiom
  }
}

void ribi::cmap::QtCommandMoveNodeTest::MoveEdgeConnectedToCenterFails() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetThreeNodeTwoEdge());
  assert(GetX(*FindFirstQtEdge(q, QtEdgeHasName("first"))) == 150);
  assert(GetY(*FindFirstQtEdge(q, QtEdgeHasName("first"))) == 225);
  try
  {
    QtNode * const first_qtnode = FindFirstQtNode(
      q,
      [name = "first", &q](QtNode * const qtnode)
      {
        return name == GetText(*qtnode);
      }
    );
    q.DoCommand(new CommandMoveNode(q, first_qtnode, 25, 125));
    QVERIFY(!"Should not get here"); //!OCLINT accepted idiom
  }
  catch (std::exception&)
  {
    QVERIFY("OK"); //!OCLINT accepted idiom
  }
}

void ribi::cmap::QtCommandMoveNodeTest::MoveQtNode() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetLonelyNode());
  const double x{GetX(*GetFirstQtNode(q))};
  const double y{GetY(*GetFirstQtNode(q))};
  assert(x == GetX(*GetFirstQtNode(q)));
  assert(y == GetY(*GetFirstQtNode(q)));
  q.DoCommand(new CommandMoveNode(q, GetFirstQtNode(q), 12, 34));
  assert(x != GetX(*GetFirstQtNode(q)));
  assert(y != GetY(*GetFirstQtNode(q)));
}

void ribi::cmap::QtCommandMoveNodeTest::MoveQtNodeAndUndo() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetLonelyNode());
  const double x{GetX(*GetFirstQtNode(q))};
  const double y{GetY(*GetFirstQtNode(q))};
  assert(x == GetX(*GetFirstQtNode(q)));
  assert(y == GetY(*GetFirstQtNode(q)));
  q.DoCommand(new CommandMoveNode(q, GetFirstQtNode(q), 12, 34));
  assert(x != GetX(*GetFirstQtNode(q)));
  assert(y != GetY(*GetFirstQtNode(q)));
  q.Undo();
  assert(x == GetX(*GetFirstQtNode(q)));
  assert(y == GetY(*GetFirstQtNode(q)));
}

void ribi::cmap::QtCommandMoveNodeTest::MoveQtNodeWithName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetThreeNodeTwoEdge());
  assert(GetX(*FindFirstQtNode(q, QtNodeHasName("one"))) == 300);
  assert(GetY(*FindFirstQtNode(q, QtNodeHasName("one"))) == 250);
  QtNode * const first_qtnode = FindFirstQtNode(q, QtNodeHasName("one"));
  q.DoCommand(new CommandMoveNode(q, first_qtnode, 25, 125));
  QVERIFY(GetX(*FindFirstQtNode(q, QtNodeHasName("one"))) == 325);
  QVERIFY(GetY(*FindFirstQtNode(q, QtNodeHasName("one"))) == 375);
}

void ribi::cmap::QtCommandMoveNodeTest
  ::MoveQtNodeWithPosition() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetThreeNodeTwoEdge());
  assert(GetX(*FindFirstQtNode(q, QtNodeHasName("one"))) == 300);
  assert(GetY(*FindFirstQtNode(q, QtNodeHasName("one"))) == 250);

  QtNode * const first_qtnode = FindFirstQtNode(
    q,
    [](QtNode * const qtnode)
    {
      return GetX(*qtnode) == 300 && GetY(*qtnode) == 250;
    }
  );
  q.DoCommand(new CommandMoveNode(q, first_qtnode, 25, 125));
  QVERIFY(GetX(*FindFirstQtNode(q, QtNodeHasName("one"))) == 325);
  QVERIFY(GetY(*FindFirstQtNode(q, QtNodeHasName("one"))) == 375);
}

void ribi::cmap::QtCommandMoveNodeTest::Parse() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  const auto c = ParseCommandMoveNode(q, "move_node(one, 10, 20)");
  QVERIFY(c != nullptr);
  QVERIFY(c->GetDx() == 10.0);
  QVERIFY(c->GetDy() == 20.0);
}

void ribi::cmap::QtCommandMoveNodeTest::ParseMoveAbsentNodeReturnsNullptr() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  const auto c = ParseCommandMoveNode(q, "move_node(absent, 10, 20)");
  QVERIFY(!c);
}

void ribi::cmap::QtCommandMoveNodeTest::ParseMoveCenterNodeReturnsNullptr() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetLonelyCenterNode());
  const auto c = ParseCommandMoveNode(q, "move_node(center, 10, 20)");
  QVERIFY(!c);
}

void ribi::cmap::QtCommandMoveNodeTest::ParseNonsenseFails() const noexcept
{
  QtConceptMap q;
  QVERIFY(ParseCommandMoveNode(q, "nonsense") == nullptr);
}
