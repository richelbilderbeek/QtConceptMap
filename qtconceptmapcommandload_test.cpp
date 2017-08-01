#include "qtconceptmapcommandload_test.h"

#include "qtconceptmapcommandload.h"
#include "qtconceptmap.h"

void ribi::cmap::qtconceptmapcommandload_test::parse() const noexcept
{
  QtConceptMap q;
  QVERIFY(parse_command_load(q, "nonsense") == nullptr);
  {
    const std::string filename{std::string(__func__) + ".cmp"};
    const auto c = parse_command_load(q, std::string("load(") + filename + ")");
    QVERIFY(c != nullptr);
    QVERIFY(c->GetFilename() == filename);
  }
}
