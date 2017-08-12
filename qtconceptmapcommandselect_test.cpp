#include "qtconceptmapcommandselect_test.h"

#include "qtconceptmapcommandselect.h"
#include "qtconceptmap.h"
#include "conceptmapfactory.h"

void ribi::cmap::QtConceptMapCommandSelectTest::SelectAbsentItemFails() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  try
  {
    q.DoCommand(new CommandSelect(q, "absent"));
    QVERIFY(!"Should not get here");
  }
  catch (std::exception&)
  {
    QVERIFY("OK");
  }
}

void ribi::cmap::QtConceptMapCommandSelectTest::SelectQtCenterNodeByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  q.DoCommand(new CommandSelect(q, "center"));
  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 1);
}

void ribi::cmap::QtConceptMapCommandSelectTest::SelectQtEdgeByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  q.DoCommand(new CommandSelect(q, "first"));
  assert(CountSelectedQtNodes(q) == 0);
  qCritical() << "CountSelectedQtEdges(q): " << CountSelectedQtEdges(q);
  //assert(CountSelectedQtEdges(q) == 1);
  QVERIFY(CountSelectedQtEdges(q) == 1);

}

void ribi::cmap::QtConceptMapCommandSelectTest::SelectQtNodeByName() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  q.DoCommand(new CommandSelect(q, "one"));
  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 1);
}

void ribi::cmap::QtConceptMapCommandSelectTest::Parse() const noexcept
{
  QtConceptMap q;
  const auto c = ParseCommandSelect(q, "select(my text)");
  QVERIFY(c != nullptr);
  QVERIFY(c->GetName() == "my text");
}

void ribi::cmap::QtConceptMapCommandSelectTest::ParseNonsenseFails() const noexcept
{
  QtConceptMap q;
  QVERIFY(ParseCommandSelect(q, "nonsense") == nullptr);
}
