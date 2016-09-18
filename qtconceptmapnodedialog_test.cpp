#include "qtconceptmapnodedialog_test.h"
#include "qtconceptmapnodedialog.h"

#include <cmath>
#include <boost/bind/bind.hpp>
#include <boost/lambda/lambda.hpp>

#include "conceptmapcompetencies.h"
#include "conceptmapconcept.h"
#include "conceptmapconceptfactory.h"
#include "conceptmapexamples.h"
#include "conceptmapnode.h"
#include "conceptmapnodefactory.h"
#include "qtconceptmapconceptdialog.h"
#include "qtconceptmapexamplesdialog.h"
#include "ui_qtconceptmapnodedialog.h"



void ribi::cmap::qtconceptmapnodedialog_test::all_tests()
{
  using namespace ribi::cmap;
  QtNodeDialog dialog;
  Node node(NodeFactory().GetTest(1));
  dialog.SetNode(node);
  //if (verbose) { TRACE("X of QtNode and QtNodeDialog must match at start"); }
  {
    QVERIFY(std::abs(dialog.GetUiX() - node.GetX()) < 2.0);
  }
}
