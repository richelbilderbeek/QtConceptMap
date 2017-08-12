#include "qtconceptmapcommandselect_test.h"

#include "qtconceptmapcommandselect.h"
#include "qtconceptmap.h"

void ribi::cmap::QtConceptMapCommandSelectTest::parse() const noexcept
{
  QtConceptMap q;

  QVERIFY(parse_command_select(q, "nonsense") == nullptr);
  {
    const auto c = parse_command_select(q, "select(my text)");
    QVERIFY(c != nullptr);
  }

}
