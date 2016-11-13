#include <iostream>
#include <QtTest/QtTest>
#include "qtconceptmapbrushfactory_test.h"
#include "qtconceptmapcenternode_test.h"
#include "qtconceptmapcompetency_test.h"
#include "qtconceptmapconceptdialog_test.h"
#include "qtconceptmapconcepteditdialog_test.h"
#include "qtconceptmapedgedialog_test.h"
#include "qtconceptmapexampledialog_test.h"
#include "qtconceptmapexamplesdialog_test.h"
#include "qtconceptmaphelper_test.h"
#include "qtconceptmapitemhighlighter_test.h"
#include "qtconceptmapnewarrow_test.h"
#include "qtconceptmapnodedialog_test.h"
#include "qtconceptmapqtedge_test.h"
#include "qtconceptmapqtnodefactory_test.h"
#include "qtconceptmapqtnode_test.h"
#include "qtconceptmaprateconceptdialog_test.h"
#include "qtconceptmaprateconcepttallydialog_test.h"
#include "qtconceptmapratedconceptdialog_test.h"
#include "qtconceptmaprateexamplesdialog_test.h"
#include "qtconceptmaprating_test.h"
#include "qtconceptmap_test.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  int error = 0;
  { ribi::cmap::qtconceptmap_test t; error |= QTest::qExec(&t, argc, argv); }
  return 1;

  //These are all tests in alphabetical order
  { ribi::cmap::qtconceptmapbrushfactory_test t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::cmap::qtconceptmapcenternode_test t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::cmap::qtconceptmapcompetency_test t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::cmap::qtconceptmapconceptdialog_test t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::cmap::qtconceptmapconcepteditdialog_test t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::cmap::qtconceptmapedgedialog_test t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::cmap::qtconceptmapexampledialog_test t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::cmap::qtconceptmapexamplesdialog_test t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::cmap::qtconceptmaphelper_test t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::cmap::qtconceptmapitemhighlighter_test t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::cmap::qtconceptmapnewarrow_test t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::cmap::qtconceptmapnodedialog_test t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::cmap::qtconceptmapqtedge_test t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::cmap::qtconceptmapqtnodefactory_test t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::cmap::qtconceptmapqtnode_test t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::cmap::qtconceptmaprateconceptdialog_test t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::cmap::qtconceptmaprateconcepttallydialog_test t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::cmap::qtconceptmapratedconceptdialog_test t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::cmap::qtconceptmaprateexamplesdialog_test t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::cmap::qtconceptmaprating_test t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::cmap::qtconceptmap_test t; error |= QTest::qExec(&t, argc, argv); }

  if (error == 0) { std::cout << "Pass\n"; }
  if (error != 0) { std::cout << "Fail\n"; }
  return error;
}
