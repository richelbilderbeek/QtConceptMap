#include "qtconceptmapconceptdialog_test.h"
#include "qtconceptmapconceptdialog.h"

#include <cassert>

void ribi::cmap::QtConceptMapConceptDialogTest::just_show()
{
  QtConceptDialog d;
  d.show();
}

void ribi::cmap::QtConceptMapConceptDialogTest::set_ui_name()
{
  QtConceptDialog d;
  d.show();
  const std::string s{"some UI name"};
  assert(d.GetUiName() != s);
  d.SetUiName(s);
  QVERIFY(d.GetUiName() == s);
}
