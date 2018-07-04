#include "qtconceptmapcommandcreatenewnode_test.h"

#include "qtconceptmapcommandcreatenewnode.h"
#include "qtconceptmap.h"

void ribi::cmap::QtConceptMapCommandCreateNewNodeTest::Parse() const noexcept
{
  QtConceptMap q;
  QVERIFY(ParseCommandCreateNewNode(q, "nonsense") == nullptr);
  {
    const auto c = ParseCommandCreateNewNode(q, "create_new_node(my text, false, 10, 20)");
    QVERIFY(c);
    assert(c);
    q.DoCommand(c);
    QSKIP("ParseCommandCreateNewNode must be parsed correctly (EASY)", "");
    QVERIFY( (c->GetX() - 10.0) < 1.0);
    QVERIFY( (c->GetY() - 20.0) < 1.0);
    QVERIFY(c->GetText() == "my text");
  }

}
