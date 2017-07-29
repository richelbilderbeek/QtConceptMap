#include "qtconceptmapcommandcreatenewnode_test.h"

#include "qtconceptmapcommandcreatenewnode.h"

void ribi::cmap::qtconceptmapcommadcreatenewnode_test::parse() const noexcept
{
  QVERIFY(parse_command_create_new_node("nonsense") == nullptr);

}
