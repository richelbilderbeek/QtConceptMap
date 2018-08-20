#include "qtconceptmapnumberedcolumn_test.h"

#include <cassert>

#include "qtconceptmapqnumberedcolumn.h"

using namespace ribi::cmap;

void ribi::cmap::QtNumberedColumnTest::Construction()
{
  QNumberedColumn w( {} );
  w.show();
  QVERIFY(!w.isHidden());
}

void ribi::cmap::QtNumberedColumnTest::EmptyRowIsDeleted()
{
  QNumberedColumn w( { "A", "B", "C" } );
  w.show();
  assert(w.rowCount() == 3);
  w.item(1, 0)->setText("");
  QVERIFY(w.rowCount() == 2);
}

void ribi::cmap::QtNumberedColumnTest::TabChangesFocus()
{
  QNumberedColumn w( { "A", "B" } );
  w.show();
  QKeyEvent e(QEvent::KeyPress, Qt::Key_Tab, Qt::NoModifier);
  w.keyPressEvent(&e);
  QVERIFY(e.isAccepted());
}
