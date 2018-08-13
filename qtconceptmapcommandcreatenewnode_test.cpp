#include "qtconceptmapcommandcreatenewnode_test.h"

#include "qtconceptmapcommandcreatenewnode.h"
#include "qtconceptmapcommands.h"
#include "qtconceptmap.h"

void ribi::cmap::QtConceptMapCommandCreateNewNodeTest
  ::CreateNewCenterNode() const noexcept
{
  QtConceptMap q;
  ProcessCommands(q,
    {
      "--command",
      "set_mode(edit); create_new_node(my text, 10, 20, center)"
    }
  );
  QVERIFY(CountQtNodes(q) == 1);
}

void ribi::cmap::QtConceptMapCommandCreateNewNodeTest
  ::CreateNewNode() const noexcept
{
  QtConceptMap q;
  ProcessCommands(q,
    {
      "--command",
      "set_mode(edit); create_new_node(my text, 10, 20, normal)"
    }
  );
  QVERIFY(CountQtNodes(q) == 1);
}

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
    QVERIFY(CountQtNodes(q) == 0);
    q.DoCommand(c);
    QVERIFY( (c->GetX() - 10.0) < 1.0);
    QVERIFY( (c->GetY() - 20.0) < 1.0);
    QVERIFY(c->GetText() == "my text");
    QVERIFY(CountQtNodes(q) == 1);
  }

}
