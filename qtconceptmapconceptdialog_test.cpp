#include "qtconceptmapconceptdialog_test.h"
#include "qtconceptmapconceptdialog.h"

#include <cassert>

void ribi::cmap::qtconceptmapconceptdialog_test::just_show()
{
  QtConceptDialog d;
  d.show();
}

void ribi::cmap::qtconceptmapconceptdialog_test::set_ui_name()
{
  QtConceptDialog d;
  d.show();
  const std::string s{"some UI name"};
  assert(d.GetUiName() != s);
  d.SetUiName(s);
  QVERIFY(d.GetUiName() == s);
}
