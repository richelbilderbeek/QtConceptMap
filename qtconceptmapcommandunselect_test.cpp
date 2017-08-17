#include "qtconceptmapcommandunselect_test.h"

#include "qtconceptmapcommandunselect.h"
#include "qtconceptmap.h"

void ribi::cmap::QtConceptMapCommandUnselectTest::Parse() const noexcept
{
  QtConceptMap q;

  QVERIFY(ParseCommandUnselect(q, "nonsense") == nullptr);
  {
    const auto c = ParseCommandUnselect(q, "unselect(my text)");
    QVERIFY(c != nullptr);
  }

}
