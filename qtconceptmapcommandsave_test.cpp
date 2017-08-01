#include "qtconceptmapcommandsave_test.h"

#include "qtconceptmapcommandsave.h"
#include "qtconceptmap.h"

void ribi::cmap::qtconceptmapcommandsave_test::parse() const noexcept
{
  QtConceptMap q;
  QVERIFY(parse_command_save(q, "nonsense") == nullptr);
  {
    const std::string filename{std::string(__func__) + ".cmp"};
    if (QFile::exists(filename.c_str())) { QFile::remove(filename.c_str()); }

    const auto c = parse_command_save(q, std::string("save(") + filename + ")");
    QVERIFY(c != nullptr);
    QVERIFY(c->GetFilename() == filename);

    assert(!QFile::exists(filename.c_str()));
    q.DoCommand(c);
    QVERIFY(QFile::exists(filename.c_str()));
    QFile::remove(filename.c_str());
    assert(!QFile::exists(filename.c_str()));
  }

}
