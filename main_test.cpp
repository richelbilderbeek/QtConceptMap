#include <cassert>
#include <iostream>
#include <QtTest/QtTest>
#include "qtconceptmapbrushfactory_test.h"
#include "qtconceptmapcommandcreatenewedge_test.h"
#include "qtconceptmapcommandcreatenewnode_test.h"
#include "qtconceptmapcommandload_test.h"
#include "qtconceptmapcommandmove_test.h"
#include "qtconceptmapcommandsave_test.h"
#include "qtconceptmapcommandselect_test.h"
#include "qtconceptmapcommandsetmode_test.h"
#include "qtconceptmapcommands_test.h"
#include "qtconceptmapcommandunselect_test.h"
#include "qtconceptmapcompetency_test.h"
#include "qtconceptmapconceptdialog_test.h"
#include "qtconceptmapconcepteditdialog_test.h"
#include "qtconceptmapexampledialog_test.h"
#include "qtconceptmapexamplesdialog_test.h"
#include "qtconceptmaphelper_test.h"
#include "qtconceptmapitemhighlighter_test.h"
#include "qtconceptmapmode_test.h"
#include "qtconceptmapnewarrow_test.h"
#include "qtconceptmapqtedge_test.h"
#include "qtconceptmapqtnodefactory_test.h"
#include "qtconceptmapqtnode_test.h"
#include "qtconceptmaprateconceptdialog_test.h"
#include "qtconceptmaprateconcepttallydialog_test.h"
#include "qtconceptmapratedconceptdialog_test.h"
#include "qtconceptmaprateexamplesdialog_test.h"
#include "qtconceptmaprating_test.h"
#include "qtconceptmap_test.h"

using namespace ribi::cmap;

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  int error = 0;
  //{ qtconceptmap_test t; error |= QTest::qExec(&t, argc, argv); }
  //{ qtconceptmapcommandmove_test t; error |= QTest::qExec(&t, argc, argv); }
  //{ qtconceptmapcommandload_test t; error |= QTest::qExec(&t, argc, argv); }
  //{ qtconceptmapcommandsave_test t; error |= QTest::qExec(&t, argc, argv); }
  //{ qtconceptmapcommands_test t; error |= QTest::qExec(&t, argc, argv); }
  //assert(!"Don't forget to let Travis test everything");

  //These are all tests in alphabetical order
  { qtconceptmapbrushfactory_test t; error |= QTest::qExec(&t, argc, argv); }
  { qtconceptmapcommandcreatenewedge_test t; error |= QTest::qExec(&t, argc, argv); }
  { qtconceptmapcommandcreatenewnode_test t; error |= QTest::qExec(&t, argc, argv); }
  { qtconceptmapcommandload_test t; error |= QTest::qExec(&t, argc, argv); }
  { qtconceptmapcommandmove_test t; error |= QTest::qExec(&t, argc, argv); }
  { qtconceptmapcommandsave_test t; error |= QTest::qExec(&t, argc, argv); }
  { qtconceptmapcommandselect_test t; error |= QTest::qExec(&t, argc, argv); }
  { qtconceptmapcommandsetmode_test t; error |= QTest::qExec(&t, argc, argv); }
  { qtconceptmapcommands_test t; error |= QTest::qExec(&t, argc, argv); }
  { qtconceptmapcommandunselect_test t; error |= QTest::qExec(&t, argc, argv); }
  { qtconceptmapcompetency_test t; error |= QTest::qExec(&t, argc, argv); }
  { qtconceptmapconceptdialog_test t; error |= QTest::qExec(&t, argc, argv); }
  { qtconceptmapconcepteditdialog_test t; error |= QTest::qExec(&t, argc, argv); }
  { qtconceptmapexampledialog_test t; error |= QTest::qExec(&t, argc, argv); }
  { qtconceptmapexamplesdialog_test t; error |= QTest::qExec(&t, argc, argv); }
  { qtconceptmaphelper_test t; error |= QTest::qExec(&t, argc, argv); }
  { qtconceptmapitemhighlighter_test t; error |= QTest::qExec(&t, argc, argv); }
  { qtconceptmapmode_test t; error |= QTest::qExec(&t, argc, argv); }
  { qtconceptmapnewarrow_test t; error |= QTest::qExec(&t, argc, argv); }
  { qtconceptmapqtedge_test t; error |= QTest::qExec(&t, argc, argv); }
  { qtconceptmapqtnodefactory_test t; error |= QTest::qExec(&t, argc, argv); }
  { qtconceptmapqtnode_test t; error |= QTest::qExec(&t, argc, argv); }
  { qtconceptmaprateconceptdialog_test t; error |= QTest::qExec(&t, argc, argv); }
  { qtconceptmaprateconcepttallydialog_test t; error |= QTest::qExec(&t, argc, argv); }
  { qtconceptmapratedconceptdialog_test t; error |= QTest::qExec(&t, argc, argv); }
  { qtconceptmaprateexamplesdialog_test t; error |= QTest::qExec(&t, argc, argv); }
  { qtconceptmaprating_test t; error |= QTest::qExec(&t, argc, argv); }
  { qtconceptmap_test t; error |= QTest::qExec(&t, argc, argv); }

  if (error == 0) { std::cout << "Pass\n"; }
  if (error != 0) { std::cout << "Fail\n"; }
  return error;
}
