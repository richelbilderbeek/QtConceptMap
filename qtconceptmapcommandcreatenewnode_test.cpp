#include "qtconceptmapcommandcreatenewnode_test.h"

#include "qtconceptmapcommandcreatenewnode.h"

void ribi::cmap::qtconceptmapcommandcreatenewnode_test::parse() const noexcept
{
  QVERIFY(parse_command_create_new_node("nonsense") == nullptr);



  {
    const auto c = parse_command_create_new_node("create_new_node(10, 20, my text)");
    QVERIFY(c != nullptr);

}
