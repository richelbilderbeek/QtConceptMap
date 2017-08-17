#include "qtconceptmapcommandunselectall_test.h"

#ifdef FIRST_FINISH_COMMAND_SELECT_ALL

#include "qtconceptmapcommandunselectall.h"
#include "qtconceptmap.h"

void ribi::cmap::QtConceptMapCommandUnselectAllTest::parse() const noexcept
{
  QtConceptMap q;

  QVERIFY(ParseCommandUnselectAll(q, "nonsense") == nullptr);
  {
    const auto c = ParseCommandUnselectAll(q, "unselect(my text)");
    QVERIFY(c != nullptr);
  }

}
#endif // FIRST_FINISH_COMMAND_SELECT_ALL
