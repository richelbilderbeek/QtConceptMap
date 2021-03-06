#include "qtconceptmapcommandcreatenewnode_test.h"

#include "qtconceptmapcommandcreatenewnode.h"
#include "qtconceptmapcommands.h"
#include "qtconceptmap.h"

void ribi::cmap::QtCommandCreateNewNodeTest
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
  QVERIFY(CountQtCenterNodes(q) == 1);
}

void ribi::cmap::QtCommandCreateNewNodeTest
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
  QVERIFY(CountQtCenterNodes(q) == 0);
}

void ribi::cmap::QtCommandCreateNewNodeTest::Parse() const noexcept
{
  QtConceptMap q;
  {
    QVERIFY(ParseCommandCreateNewNode(q, "nonsense") == nullptr);
    QVERIFY(ParseCommandCreateNewNode(q, "create_new_node(nonsense") == nullptr);
    QVERIFY(ParseCommandCreateNewNode(q, "create_new_node(nonsense)") == nullptr);
    QVERIFY(ParseCommandCreateNewNode(q, "create_new_node(non, sense)") == nullptr);
    QVERIFY(ParseCommandCreateNewNode(q, "create_new_node(non, sen, se)") == nullptr);
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
