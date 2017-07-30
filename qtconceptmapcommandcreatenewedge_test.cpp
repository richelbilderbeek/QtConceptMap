#include "qtconceptmapcommandcreatenewedge_test.h"

#include "qtconceptmapcommandcreatenewedge.h"
#include "qtconceptmapcommandcreatenewnode.h"
#include "qtconceptmap.h"

#include <QDebug>

void ribi::cmap::qtconceptmapcommandcreatenewedge_test::parse() const noexcept
{
  QtConceptMap q;
  QVERIFY(parse_command_create_new_edge(q, "nonsense") == nullptr);
  {
    const auto c1 = parse_command_create_new_node(q, "create_new_node(10, 20, my text)");
    q.DoCommand(c1);
    const auto c2 = parse_command_create_new_node(q, "create_new_node(10, 40, my text)");
    q.DoCommand(c2);
    const auto c3 = parse_command_create_new_edge(q, "create_new_edge(my text)");
    QVERIFY(c3 != nullptr);
    QVERIFY(GetText(*c3) == "my text");
    q.DoCommand(c3);
  }
}
