#include "qtconceptmapcommandtogglearrowtail_test.h"

#include "qtconceptmapcommandtogglearrowtail.h"
#include "qtconceptmapcommandcreatenewnode.h"
#include "qtconceptmap.h"

#include <QDebug>

void ribi::cmap::QtConceptMapCommandToggleArrowTailTest::Parse() const noexcept
{
  QtConceptMap q;
  QVERIFY(ParseCommandToggleArrowTail(q, "toggle_arrow_tail()") != nullptr);
}

void ribi::cmap::QtConceptMapCommandToggleArrowTailTest::ParseNonsenseFails() const noexcept
{
  QtConceptMap q;
  QVERIFY(ParseCommandToggleArrowTail(q, "nonsense") == nullptr);
}
