#include "qtconceptmapcommandcreatenewnode_test.h"

#include "qtconceptmapcommandcreatenewnode.h"
#include "qtconceptmap.h"

void ribi::cmap::QtConceptMapCommandCreateNewNodeTest::Parse() const noexcept
{
  QtConceptMap q;
  {
    QVERIFY(ParseCommandCreateNewNode(q, "nonsense") == nullptr);
  }
  {
    const auto c = ParseCommandCreateNewNode(
      q, "create_new_node(my text, 10, 20, normal)"
    );
    QVERIFY(c);
    assert(c);
    q.DoCommand(c);
    QVERIFY( (c->GetX() - 10.0) < 1.0);
    QVERIFY( (c->GetY() - 20.0) < 1.0);
    QVERIFY(c->GetText() == "my text");
  }

}
