#include "qtconceptmapqtnodedialog_test.h"
#include "qtconceptmapqtnodedialog.h"


#include <cmath>
#include <sstream>

#include <boost/lambda/lambda.hpp>

#include <QDesktopWidget>
#include <QVBoxLayout>

#include "conceptmapnode.h"
#include "conceptmapnodefactory.h"
#include "qtconceptmapnodedialog.h"
#include "qtconceptmapqtnodefactory.h"
#include "qtconceptmapqtnode.h"
#include "qtroundededitrectitemdialog.h"




#include "ui_qtconceptmapqtnodedialog.h"

void ribi::cmap::qtconceptmapqtnodedialog_test::all_tests()
{
  using namespace ribi::cmap;
  QtQtNodeDialog dialog;
  const auto node = NodeFactory().GetTest(1);
  const boost::shared_ptr<QtNode> qtnode{new QtNode(node)};
  dialog.SetQtNode(qtnode);
  //if (verbose) { TRACE("SetUiX and GetUiX must be symmetric"); }
  {
    const double old_x{dialog.GetUiX()};
    const double new_x{old_x + 10.0};
    dialog.SetUiX(new_x);
    QVERIFY(std::abs(dialog.GetUiX() - new_x) < 2.0);
  }
  //if (verbose) { TRACE("SetUiY and GetUiY must be symmetric"); }
  {
    const double old_y{dialog.GetUiY()};
    const double new_y{old_y + 10.0};
    dialog.SetUiY(new_y);
    QVERIFY(std::abs(dialog.GetUiY() - new_y) < 2.0);
  }
}
