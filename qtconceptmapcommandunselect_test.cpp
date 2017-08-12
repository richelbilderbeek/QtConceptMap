#include "qtconceptmapcommandunselect_test.h"

#include "qtconceptmapcommandunselect.h"
#include "qtconceptmap.h"

void ribi::cmap::QtConceptMapCommandUnselectTest::parse() const noexcept
{
  QtConceptMap q;

  QVERIFY(parse_command_unselect(q, "nonsense") == nullptr);
  {
    const auto c = parse_command_unselect(q, "unselect(my text)");
    QVERIFY(c != nullptr);
  }

}
