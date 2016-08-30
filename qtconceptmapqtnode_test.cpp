#include "qtconceptmapqtnode_test.h"
#include "qtconceptmapqtnode.h"

#include <climits>
#include <cmath>

#include <boost/lambda/lambda.hpp>
#include <boost/signals2.hpp>

#include <QCursor>
#include <QKeyEvent>
#include <QPainter>
#include <QPen>

#include "conceptmapconcept.h"
#include "conceptmapexamples.h"
#include "conceptmapnodefactory.h"
#include "conceptmapnode.h"
#include "container.h"
#include "qtconceptmapbrushfactory.h"
#include "qtconceptmapqtnodefactory.h"

#include "trace.h"


void ribi::cmap::qtconceptmapqtnode_test::all_tests()
{
  using namespace ribi;
  using namespace ribi::cmap;
  const bool verbose{false};
  const double max_error = 2.0;

  if (verbose) { TRACE("QtNode can be converted to QtRoundedEditRectItem"); }
  {
    const auto qtnode = QtNodeFactory().GetTest(1);
    const boost::shared_ptr<QtRoundedEditRectItem> edit_rect{boost::dynamic_pointer_cast<QtRoundedEditRectItem>(qtnode)};
    QVERIFY(edit_rect.get());
    QVERIFY(qtnode == edit_rect);
  }

  if (verbose) { TRACE("Test X coordinat in Node and QtRoundedEditRectItem being equal at creation"); }
  {
    const auto qtnode = QtNodeFactory().GetTest(1);
    const boost::shared_ptr<QtRoundedEditRectItem> edit_rect{boost::dynamic_pointer_cast<QtRoundedEditRectItem>(qtnode)};
    const auto node = qtnode->GetNode();
    const double node_x = node.GetX();
    const double edit_rect_x = edit_rect->GetCenterX();
    QVERIFY(std::abs(node_x - edit_rect_x) < max_error);
  }
  if (verbose) { TRACE("Test Y coordinat in Node and QtRoundedEditRectItem being equal at creation"); }
  {
    const auto qtnode = QtNodeFactory().GetTest(1);
    const boost::shared_ptr<QtRoundedEditRectItem> edit_rect{boost::dynamic_pointer_cast<QtRoundedEditRectItem>(qtnode)};
    const auto node = qtnode->GetNode();
    const double node_y = node.GetY();
    const double edit_rect_y = edit_rect->GetCenterY();
    QVERIFY(std::abs(node_y - edit_rect_y) < max_error);
  }
}
