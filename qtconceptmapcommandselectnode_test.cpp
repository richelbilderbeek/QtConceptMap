#include "qtconceptmapcommandselectnode_test.h"

#include "qtconceptmapcommandselectnode.h"
#include "qtconceptmap.h"
#include "conceptmapfactory.h"

void ribi::cmap::QtConceptMapCommandSelectNodeTest::SelectAbsentItemFails() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  try
  {
    QtNode * const first_qtnode = FindFirstQtNode(q, QtNodeHasName("absent"));
    q.DoCommand(new CommandSelectNode(q, first_qtnode));
    QVERIFY(!"Should not get here");
  }
  catch (std::exception&)
  {
    QVERIFY("OK");
  }
}

void ribi::cmap::QtConceptMapCommandSelectNodeTest::SelectQtCenterNodeByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  QtNode * const first_qtnode = FindFirstQtNode(q, QtNodeHasName("center"));
  q.DoCommand(new CommandSelectNode(q, first_qtnode));

  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 1);
}

void ribi::cmap::QtConceptMapCommandSelectNodeTest
  ::SelectOneOfTwoQtNodesByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);

  QtNode * const first_qtnode = FindFirstQtNode(q, QtNodeHasName("one"));
  q.DoCommand(new CommandSelectNode(q, first_qtnode));

  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 1);
}

void ribi::cmap::QtConceptMapCommandSelectNodeTest
  ::SelectTwoQtNodesByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  QtNode * const first_qtnode = FindFirstQtNode(q, QtNodeHasName("one"));
  QtNode * const second_qtnode = FindFirstQtNode(q, QtNodeHasName("center"));
  q.DoCommand(new CommandSelectNode(q, first_qtnode));
  q.DoCommand(new CommandSelectNode(q, second_qtnode));
  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 2);
}

void ribi::cmap::QtConceptMapCommandSelectNodeTest::Parse() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  const auto c = ParseCommandSelectNode(q, "select_node(one)");
  assert(c);
  QVERIFY(c != nullptr);
}

void ribi::cmap::QtConceptMapCommandSelectNodeTest::ParseNonsenseFails() const noexcept
{
  QtConceptMap q;
  QVERIFY(ParseCommandSelectNode(q, "nonsense") == nullptr);
}
