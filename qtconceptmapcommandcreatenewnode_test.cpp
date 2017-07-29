#include "qtconceptmapcommandcreatenewnode_test.h"

#include "qtconceptmapcommandcreatenewnode.h"
#include "qtconceptmap.h"

void ribi::cmap::qtconceptmapcommandcreatenewnode_test::parse() const noexcept
{
  QtConceptMap q;
  QVERIFY(parse_command_create_new_node(q, "nonsense") == nullptr);
  {
    const auto c = parse_command_create_new_node(q, "create_new_node(10, 20, my text)");
    QVERIFY(c != nullptr);
    QVERIFY( (c->GetX() - 10.0) < 1.0);
    QVERIFY( (c->GetY() - 20.0) < 1.0);
    QVERIFY(c->GetText() == "my text");
  }

}
