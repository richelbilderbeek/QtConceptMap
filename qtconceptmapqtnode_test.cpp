#include "qtconceptmapqtnode_test.h"
#include "qtconceptmapqtnode.h"

#include <climits>
#include <cmath>
#include <iostream>
#include <sstream>

//#include <QCursor>
//#include <QKeyEvent>
//#include <QPainter>
//#include <QPen>

//#include "conceptmapconcept.h"
//#include "conceptmapexamples.h"
//#include "conceptmapnodefactory.h"
//#include "conceptmapnode.h"
//#include "container.h"
//#include "qtconceptmapbrushfactory.h"
#include "qtconceptmapqtnodefactory.h"

void ribi::cmap::qtconceptmapqtnode_test::disable_and_enable()
{
  const std::unique_ptr<QtNode> qtnode = QtNodeFactory().GetTest(1);
  qtnode->show();
  qtnode->DisableAll();
  qtnode->show();
  qtnode->EnableAll();
  qtnode->show();
}

void ribi::cmap::qtconceptmapqtnode_test::qtnode_is_qtroundededitrectitem()
{
  const auto qtnode = QtNodeFactory().GetTest(1);
  const QtRoundedEditRectItem * edit_rect{
    dynamic_cast<QtRoundedEditRectItem*>(qtnode.get())
  };
  QVERIFY(edit_rect);
  QVERIFY(qtnode.get() == edit_rect);
}

void ribi::cmap::qtconceptmapqtnode_test::qtnode_is_qtroundededitrectitem_with_equal_x()
{
  const auto qtnode = QtNodeFactory().GetTest(1);
  const QtRoundedEditRectItem * edit_rect{
    dynamic_cast<QtRoundedEditRectItem*>(qtnode.get())
  };
  const auto node = qtnode->GetNode();
  const double node_x = node.GetX();
  const double edit_rect_x = edit_rect->GetCenterX();
  QVERIFY(std::abs(node_x - edit_rect_x) < 2.0);

}

void ribi::cmap::qtconceptmapqtnode_test::qtnode_is_qtroundededitrectitem_with_equal_y()
{
  const auto qtnode = QtNodeFactory().GetTest(1);
  const QtRoundedEditRectItem * edit_rect{
    dynamic_cast<QtRoundedEditRectItem*>(qtnode.get())
  };
  const auto node = qtnode->GetNode();
  const double node_y = node.GetY();
  const double edit_rect_y = edit_rect->GetCenterY();
  QVERIFY(std::abs(node_y - edit_rect_y) < 2.0);
}

void ribi::cmap::qtconceptmapqtnode_test::to_str()
{
  const std::unique_ptr<QtNode> qtnode = QtNodeFactory().GetTest(1);
  QVERIFY(!qtnode->ToStr().empty());
}

void ribi::cmap::qtconceptmapqtnode_test::to_stream()
{
  const std::unique_ptr<QtNode> qtnode = QtNodeFactory().GetTest(1);
  std::stringstream s;
  s << (*qtnode);
  QVERIFY(!s.str().empty());
}
