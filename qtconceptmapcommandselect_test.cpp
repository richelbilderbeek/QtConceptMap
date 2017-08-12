#include "qtconceptmapcommandselect_test.h"

#include "qtconceptmapcommandselect.h"
#include "qtconceptmap.h"

void ribi::cmap::QtConceptMapCommandSelectTest::parse() const noexcept
{
  QtConceptMap q;

  QVERIFY(ParseCommandSelect(q, "nonsense") == nullptr);
  {
    const auto c = ParseCommandSelect(q, "select(my text)");
    QVERIFY(c != nullptr);
  }

}
