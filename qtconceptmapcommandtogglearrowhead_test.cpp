#include "qtconceptmapcommandtogglearrowhead_test.h"

#include "qtconceptmapcommandtogglearrowhead.h"
#include "qtconceptmapcommandcreatenewnode.h"
#include "qtconceptmap.h"

#include <QDebug>

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
