#include <cassert>
#include <iostream>
#include <QtTest/QtTest>
#include "qtconceptmapbrushfactory_test.h"
#include "qtconceptmapclassifyexamplesdialog_test.h"
#include "qtconceptmapclassifyexamplesdialogcloser_test.h"
#include "qtconceptmapcommandcreatenewedge_test.h"
#include "qtconceptmapcommandcreatenewnode_test.h"
#include "qtconceptmapcommanddeleteselected_test.h"
#include "qtconceptmapcommandmoveedge_test.h"
#include "qtconceptmapcommandmovenode_test.h"
#include "qtconceptmapcommandmove_test.h"
#include "qtconceptmapcommandselectedge_test.h"
#include "qtconceptmapcommandselectnode_test.h"
#include "qtconceptmapcommandselect_test.h"
#include "qtconceptmapcommandsetconcept_test.h"
#include "qtconceptmapcommandsetmode_test.h"
#include "qtconceptmapcommands_test.h"
#include "qtconceptmapcommand_test.h"
#include "qtconceptmapcommandtogglearrowhead_test.h"
#include "qtconceptmapcommandtogglearrowtail_test.h"
#include "qtconceptmapcommandunselectall_test.h"
#include "qtconceptmapcommandunselectedge_test.h"
#include "qtconceptmapcommandunselectnode_test.h"
#include "qtconceptmapcommandunselect_test.h"
#include "qtconceptmapcompetency_test.h"
#include "qtconceptmapeditconceptdialogcloser_test.h"
#include "qtconceptmapeditconceptdialog_test.h"
#include "qtconceptmaphelper_test.h"
#include "qtconceptmapitemhighlighter_test.h"
#include "qtconceptmapmode_test.h"
#include "qtconceptmapnewarrow_test.h"
#include "qtconceptmapnumberedcolumn_test.h"
#include "qtconceptmapqtedge_test.h"
#include "qtconceptmapqtnodefactory_test.h"
#include "qtconceptmapqtnode_test.h"
#include "qtconceptmaprateconceptdialogcloser_test.h"
#include "qtconceptmaprateconceptdialog_test.h"
#include "qtconceptmaprateconcepttallydialogcloser_test.h"
#include "qtconceptmaprateconcepttallydialog_test.h"
#include "qtconceptmapratedconceptdialog_test.h"
#include "qtconceptmaprating_test.h"
#include "qtconceptmap_test.h"

using namespace ribi::cmap;

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  int error = 0;
  //{ QtClassifyExamplesDialogCloserTest t; error |= QTest::qExec(&t, argc, argv); }
  //assert(!"Don't forget to let Travis test everything");

  //These are all tests in alphabetical order
  { CompetencyTest t; error |= QTest::qExec(&t, argc, argv); }
  { HelperTest t; error |= QTest::qExec(&t, argc, argv); }
  { ModeTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtBrushFactoryTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtClassifyExamplesDialogTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtClassifyExamplesDialogCloserTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtCommandCreateNewEdgeTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtCommandCreateNewNodeTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtCommandDeleteSelectedTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtCommandMoveEdgeTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtCommandMoveNodeTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtCommandMoveTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtCommandSelectEdgeTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtCommandSelectNodeTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtCommandSelectTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtCommandSetConceptTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtCommandSetModeTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtCommandsTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtCommandTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtCommandToggleArrowHeadTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtCommandToggleArrowTailTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtCommandUnselectAllTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtCommandUnselectEdgeTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtCommandUnselectNodeTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtCommandUnselectTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtConceptMapTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtEdgeTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtEditConceptDialogCloserTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtEditDialogTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtItemHighlighterTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtNewArrowTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtNodeFactoryTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtNodeTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtNumberedColumnTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtRateConceptDialogCloserTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtRateConceptDialogTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtRateConceptTallyDialogCloserTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtRateConceptTallyDialogTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtRatedConceptDialogTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtRatingTest t; error |= QTest::qExec(&t, argc, argv); }

  if (error == 0) { std::cout << "Pass\n"; }
  if (error != 0) { std::cout << "Fail\n"; }
  return error;
}
