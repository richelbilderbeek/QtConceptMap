#include "qtconceptmapedgedialog_test.h"
#include "qtconceptmapedgedialog.h"


#include <cmath>
#include <boost/bind/bind.hpp>
#include <boost/lambda/lambda.hpp>

#include <QLabel>

#include "conceptmapedge.h"
#include "conceptmapedgefactory.h"
#include "conceptmapnode.h"
#include "conceptmapnodefactory.h"
#include "conceptmapconcept.h"
#include "conceptmapconceptfactory.h"
#include "qtconceptmapexampledialog.h"
#include "qtconceptmapexamplesdialog.h"
#include "qtconceptmapconceptdialog.h"
#include "qtconceptmapnodedialog.h"
#include "ui_qtconceptmapedgedialog.h"

#include "trace.h"


void ribi::cmap::qtconceptmapedgedialog_test::all_tests()
{
  using namespace ribi::cmap;
  const bool verbose{false};
  const Node from = NodeFactory().GetTest(0);
  const Node to = NodeFactory().GetTest(0);
  Edge edge = EdgeFactory().GetTest(0);
  QtEdgeDialog dialog(edge);
  if (verbose) { TRACE("X of QtNode and QtNodeDialog must match at start"); }
  {
    QVERIFY(std::abs(dialog.GetUiX() - edge.GetNode().GetX()) < 2.0);
  }
  if (verbose) { TRACE("SetX and GetX must be symmetric"); }
  {
    const double new_x{dialog.GetUiX() + 10.0};
    dialog.SetUiX(new_x);
    QVERIFY(std::abs(dialog.GetUiX() - new_x) < 1.0);
  }
  if (verbose) { TRACE("SetY and GetY must be symmetric"); }
  {
    const double new_y{dialog.GetUiY() + 10.0};
    dialog.SetUiY(new_y);
    QVERIFY(std::abs(dialog.GetUiY() - new_y) < 1.0);
  }
  if (verbose) { TRACE("If X is set via Edge, QtNodeDialog must sync"); }
  {
    const double old_x{edge.GetNode().GetX()};
    const double new_x{old_x + 10.0};
    edge.GetNode().SetX(new_x);
    QVERIFY(std::abs(new_x - dialog.GetUiX()) < 2.0);
  }
  if (verbose) { TRACE("SetUiHasHeadArrow and GetUiHasHeadArrow must be symmetric"); }
  {
    dialog.SetUiHasHeadArrow(true);
    QVERIFY(dialog.GetUiHasHeadArrow());
    dialog.SetUiHasHeadArrow(false);
    QVERIFY(!dialog.GetUiHasHeadArrow());
  }
  if (verbose) { TRACE("SetUiHasTailArrow and GetUiHasTailArrow must be symmetric"); }
  {
    dialog.SetUiHasHeadArrow(true);
    QVERIFY(dialog.GetUiHasHeadArrow());
    dialog.SetUiHasHeadArrow(false);
    QVERIFY(!dialog.GetUiHasHeadArrow());
  }
}
