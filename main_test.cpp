#include <cassert>
#include <iostream>
#include <QtTest/QtTest>
#include "qtconceptmapbrushfactory_test.h"
#include "qtconceptmapcommandcreatenewedge_test.h"
#include "qtconceptmapcommandcreatenewnode_test.h"
#include "qtconceptmapcommandload_test.h"
#include "qtconceptmapcommandmove_test.h"
#include "qtconceptmapcommandmoveedge_test.h"
#include "qtconceptmapcommandmovenode_test.h"
#include "qtconceptmapcommandsave_test.h"
#include "qtconceptmapcommandselect_test.h"
#include "qtconceptmapcommandselectedge_test.h"
#include "qtconceptmapcommandselectnode_test.h"
#include "qtconceptmapcommandsetconcept_test.h"
#include "qtconceptmapcommandsetmode_test.h"
#include "qtconceptmapcommandtogglearrowhead_test.h"
#include "qtconceptmapcommandtogglearrowtail_test.h"
#include "qtconceptmapcommands_test.h"
#include "qtconceptmapcommandunselect_test.h"
#include "qtconceptmapcommandunselectnode_test.h"
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
  //{ QtConceptMapCommandsTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtConceptMapCommandSelectNodeTest t; error |= QTest::qExec(&t, argc, argv); }
  //{ QtConceptMapCommandSelectEdgeTest t; error |= QTest::qExec(&t, argc, argv); }
  //{ QtConceptMapCommandUnselectNodeTest t; error |= QTest::qExec(&t, argc, argv); }
  //{ QtConceptMapCommandSetConceptTest t; error |= QTest::qExec(&t, argc, argv); }
  //{ QtConceptMapTest t; error |= QTest::qExec(&t, argc, argv); }
  assert(!"Don't forget to let Travis test everything");

  //These are all tests in alphabetical order
  { QtConceptMapBrushFactoryTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtConceptMapCommandCreateNewEdgeTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtConceptMapCommandCreateNewNodeTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtConceptMapCommandLoadTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtConceptMapCommandMoveEdgeTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtConceptMapCommandMoveNodeTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtConceptMapCommandMoveTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtConceptMapCommandSaveTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtConceptMapCommandSelectEdgeTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtConceptMapCommandSelectNodeTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtConceptMapCommandSelectTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtConceptMapCommandSetConceptTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtConceptMapCommandSetModeTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtConceptMapCommandsTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtConceptMapCommandToggleArrowHeadTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtConceptMapCommandToggleArrowTailTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtConceptMapCommandUnselectTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtConceptMapCommandUnselectNodeTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtConceptMapCompetencyTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtConceptMapConceptDialogTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtConceptMapConceptEditDialogTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtConceptMapExampleDialogTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtConceptMapExamplesDialogTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtConceptMapHelperTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtConceptMapItemHighlighterTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtConceptMapModeTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtConceptMapNewArrowTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtConceptMapQtEdgeTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtConceptMapQtNodeFactoryTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtConceptMapQtNodeTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtConceptMapRateConceptDialogTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtConceptMapRateConceptTallyDialogTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtConceptMapRatedConceptDialogTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtConceptMapRateExamplesDialogTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtConceptMapRatingTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtConceptMapTest t; error |= QTest::qExec(&t, argc, argv); }

  if (error == 0) { std::cout << "Pass\n"; }
  if (error != 0) { std::cout << "Fail\n"; }
  return error;
}
