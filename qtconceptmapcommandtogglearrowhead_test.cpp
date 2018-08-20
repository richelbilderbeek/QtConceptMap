#include "qtconceptmapcommandtogglearrowhead_test.h"

#include "qtconceptmapcommandtogglearrowhead.h"
#include "qtconceptmapcommandselect.h"
#include "qtconceptmap.h"
#include "conceptmapfactory.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmapqtnode.h"

#include <QDebug>

void ribi::cmap::QtCommandToggleArrowHeadTest
  ::ToggleAbsentEdgeShouldFail() const noexcept
{
  QtConceptMap q;
  try
  {
    q.DoCommand(new CommandToggleArrowHead(q));
    QVERIFY(!"Should not get here"); //!OCLINT accepted idiom
  }
  catch (std::exception&)
  {
    QVERIFY("OK"); //!OCLINT accepted idiom
  }
}

void ribi::cmap::QtCommandToggleArrowHeadTest
  ::ToggleEdge() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdgeNoCenter());
  q.DoCommand(new CommandSelect(q, *FindFirstQtEdge(q, QtEdgeHasName("second"))));
  assert(CountQtArrowHeads(q) == 0);
  q.DoCommand(new CommandToggleArrowHead(q));
  assert(CountQtArrowHeads(q) == 1);
}

void ribi::cmap::QtCommandToggleArrowHeadTest
  ::ToggleEdgeConnectedToCenterNode() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  q.DoCommand(new CommandSelect(q, *FindFirstQtEdge(q, QtEdgeHasName("first"))));
  assert(CountQtArrowHeads(q) == 1);
  q.DoCommand(new CommandToggleArrowHead(q));
  QVERIFY(CountQtArrowHeads(q) != 1);
}

void ribi::cmap::QtCommandToggleArrowHeadTest::Parse() const noexcept
{
  QtConceptMap q;

  QVERIFY(ParseCommandToggleArrowHead(q, "toggle_head()") != nullptr);
}

void ribi::cmap::QtCommandToggleArrowHeadTest::ParseNonsenseFails() const noexcept
{
  QtConceptMap q;
  QVERIFY(ParseCommandToggleArrowHead(q, "nonsense") == nullptr);
}
