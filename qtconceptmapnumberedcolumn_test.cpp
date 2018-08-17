#include "qtconceptmapnumberedcolumn_test.h"

#include <cassert>

#include "qtconceptmapqnumberedcolumn.h"

using namespace ribi::cmap;

void ribi::cmap::QtConceptMapNumberedColumnTest::Construction()
{
  QNumberedColumn w( {} );
  w.show();
  QVERIFY(!w.isHidden());
}

void ribi::cmap::QtConceptMapNumberedColumnTest::EmptyRowIsDeleted()
{
  QNumberedColumn w( { "A", "B", "C" } );
  w.show();
  assert(w.rowCount() == 3);
  w.item(1, 0)->setText("");
  QVERIFY(w.rowCount() == 2);
}

void ribi::cmap::QtConceptMapNumberedColumnTest::TabChangesFocus()
{
  QNumberedColumn w( { "A", "B" } );
  w.show();
  QKeyEvent e(QEvent::KeyPress, Qt::Key_Tab, Qt::NoModifier);
  w.keyPressEvent(&e);
  QVERIFY(e.isAccepted());
}
