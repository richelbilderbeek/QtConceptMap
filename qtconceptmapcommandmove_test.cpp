#include "qtconceptmapcommandmove_test.h"

#include "qtconceptmapcommandmove.h"
#include "qtconceptmap.h"

void ribi::cmap::qtconceptmapcommandmove_test::parse() const noexcept
{
  QtConceptMap q;

  QVERIFY(parse_command_move(q, "nonsense") == nullptr);
  {
    const auto c = parse_command_move(q, "move(node label, 10, 20)");
    QVERIFY(c != nullptr);
    QVERIFY(c->GetName() == "node label");
    QVERIFY(c->GetDx() == 10.0);
    QVERIFY(c->GetDy() == 20.0);
  }

}
