#include "qtconceptmapcommandload_test.h"

#include "qtconceptmapcommandload.h"
#include "qtconceptmap.h"

void ribi::cmap::QtConceptMapCommandLoadTest::Parse() const noexcept
{
  QtConceptMap q;
  const std::string filename{std::string(__func__) + ".cmp"};
  const auto c = ParseCommandLoad(q, std::string("load(") + filename + ")");
  QVERIFY(c != nullptr);
  QVERIFY(c->GetFilename() == filename);
}

void ribi::cmap::QtConceptMapCommandLoadTest::ParseNonsenseFails() const noexcept
{
  QtConceptMap q;
  QVERIFY(ParseCommandLoad(q, "nonsense") == nullptr);
}
