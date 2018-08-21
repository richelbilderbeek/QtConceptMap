#include "qtconceptmapcommandsave_test.h"

#include "qtconceptmapcommandsave.h"
#include "qtconceptmap.h"

void ribi::cmap::QtCommandSaveTest::Parse() const noexcept
{
  QtConceptMap q;

  const std::string filename{std::string(__func__) + ".cmp"};
  if (QFile::exists(filename.c_str())) { QFile::remove(filename.c_str()); }

  const auto c = ParseCommandSave(q, std::string("save(") + filename + ")");
  QVERIFY(c != nullptr);
  QVERIFY(c->GetFilename() == filename);

  assert(!QFile::exists(filename.c_str()));
  q.DoCommand(c);
  QVERIFY(QFile::exists(filename.c_str()));
  QFile::remove(filename.c_str());
  assert(!QFile::exists(filename.c_str()));
}

void ribi::cmap::QtCommandSaveTest::ParseNonsense() const noexcept
{
  QtConceptMap q;
  QVERIFY(ParseCommandSave(q, "nonsense") == nullptr);
}

void ribi::cmap::QtCommandSaveTest::Saves() const noexcept
{
  QtConceptMap q;
  const std::string filename{"tmp.cmp"};
  if (QFile::exists(filename.c_str())) { QFile::remove(filename.c_str()); }
  assert(!QFile::exists(filename.c_str()));
  q.DoCommand(new CommandSave(q, filename));
  QVERIFY(QFile::exists(filename.c_str()));
  QFile::remove(filename.c_str());
}
