#include "qtconceptmapcommandtogglearrowhead_test.h"

#include "qtconceptmapcommandtogglearrowhead.h"
#include "qtconceptmapcommandselect.h"
#include "qtconceptmap.h"
#include "conceptmapfactory.h"

#include <QDebug>

void ribi::cmap::QtConceptMapCommandToggleArrowHeadTest
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

void ribi::cmap::QtConceptMapCommandToggleArrowHeadTest
  ::ToggleEdge() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdgeNoCenter());
  q.DoCommand(new CommandSelect(q, "second"));
  assert(CountQtArrowHeads(q) == 0);
  q.DoCommand(new CommandToggleArrowHead(q));
  assert(CountQtArrowHeads(q) == 1);
}

void ribi::cmap::QtConceptMapCommandToggleArrowHeadTest
  ::ToggleEdgeConnectedToCenterNode() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  q.DoCommand(new CommandSelect(q, "first"));
  assert(CountQtArrowHeads(q) == 0);
  q.DoCommand(new CommandToggleArrowHead(q));
  assert(CountQtArrowHeads(q) == 1);
}

void ribi::cmap::QtConceptMapCommandToggleArrowHeadTest::Parse() const noexcept
{
  QtConceptMap q;

  QVERIFY(ParseCommandToggleArrowHead(q, "toggle_head()") != nullptr);
}

void ribi::cmap::QtConceptMapCommandToggleArrowHeadTest::ParseNonsenseFails() const noexcept
{
  QtConceptMap q;
  QVERIFY(ParseCommandToggleArrowHead(q, "nonsense") == nullptr);
}
