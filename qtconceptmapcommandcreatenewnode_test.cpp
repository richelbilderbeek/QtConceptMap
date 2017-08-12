#include "qtconceptmapcommandcreatenewnode_test.h"

#include "qtconceptmapcommandcreatenewnode.h"
#include "qtconceptmap.h"

void ribi::cmap::QtConceptMapCommandCreateNewNodeTest::parse() const noexcept
{
  QtConceptMap q;
  QVERIFY(parse_command_create_new_node(q, "nonsense") == nullptr);
  {
    const auto c = parse_command_create_new_node(q, "create_new_node(my text, false, 10, 20)");
    QVERIFY(c);
    assert(c);
    q.DoCommand(c);
    QVERIFY( (c->GetX() - 10.0) < 1.0);
    QVERIFY( (c->GetY() - 20.0) < 1.0);
    QVERIFY(c->GetText() == "my text");
  }

}
