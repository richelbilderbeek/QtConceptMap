#include "qtconceptmapcommandtogglearrowtail_test.h"

#include "conceptmapfactory.h"
#include "qtconceptmapcommandtogglearrowtail.h"
#include "qtconceptmapcommandcreatenewnode.h"
#include "qtconceptmap.h"
#include "qtconceptmapcommandselect.h"
#include "qtconceptmapqtedge.h"
#include <QDebug>

void ribi::cmap::QtCommandToggleArrowTailTest::Parse() const noexcept
{
  QtConceptMap q;
  QVERIFY(ParseCommandToggleArrowTail(q, "toggle_arrow_tail()") != nullptr);
}

void ribi::cmap::QtCommandToggleArrowTailTest::ParseNonsenseFails() const noexcept
{
  QtConceptMap q;
  QVERIFY(ParseCommandToggleArrowTail(q, "nonsense") == nullptr);
}

void ribi::cmap::QtCommandToggleArrowTailTest
  ::ToggleAbsentEdgeShouldFail() const noexcept
{
  QtConceptMap q;
  try
  {
    q.DoCommand(new CommandToggleArrowTail(q));
    QVERIFY(!"Should not get here"); //!OCLINT accepted idiom
  }
  catch (std::exception&)
  {
    QVERIFY("OK"); //!OCLINT accepted idiom
  }
}

void ribi::cmap::QtCommandToggleArrowTailTest
  ::ToggleEdge() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdgeNoCenter());
  q.DoCommand(new CommandSelect(q, *FindFirstQtEdge(q, QtEdgeHasName("second"))));
  assert(CountQtArrowTails(q) == 0);
  q.DoCommand(new CommandToggleArrowTail(q));
  assert(CountQtArrowTails(q) == 1);
}

void ribi::cmap::QtCommandToggleArrowTailTest
  ::ToggleEdgeConnectedToCenterNode() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  q.DoCommand(new CommandSelect(q, *FindFirstQtEdge(q, QtEdgeHasName("first"))));
  assert(CountQtArrowTails(q) == 0);
  q.DoCommand(new CommandToggleArrowTail(q));
  QVERIFY(CountQtArrowTails(q) != 0);
}
