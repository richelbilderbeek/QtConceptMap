#include "qtconceptmapqtnode_test.h"
#include "qtconceptmapqtnode.h"

#include <climits>
#include <cmath>
#include <iostream>
#include <sstream>

#include "qtconceptmapqtnodefactory.h"

void ribi::cmap::QtConceptMapQtNodeTest::center_qtnode_is_center_node()
{
  const auto qtnode = QtNodeFactory().GetCenter();
  QVERIFY(IsCenterNode(*qtnode));
}

void ribi::cmap::QtConceptMapQtNodeTest::normal_qtnode_is_not_center_node()
{

  const auto qtnode = QtNodeFactory().GetNormal();
  QVERIFY(!IsCenterNode(*qtnode));
}

void ribi::cmap::QtConceptMapQtNodeTest::disable_and_enable()
{
  const auto qtnode = QtNodeFactory().GetTest(1);
  qtnode->show();
  qtnode->DisableAll();
  qtnode->show();
  qtnode->EnableAll();
  qtnode->show();
}

void ribi::cmap::QtConceptMapQtNodeTest::press_nonsense_is_rejected()
{
  const auto qtnode = QtNodeFactory().GetTest(1);
  QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_F4, Qt::NoModifier);
  qtnode->keyPressEvent(event);
  QVERIFY(!event->isAccepted());
}

void ribi::cmap::QtConceptMapQtNodeTest::qtnode_is_qtroundededitrectitem()
{
  const auto qtnode = QtNodeFactory().GetTest(1);
  const QtRoundedEditRectItem * edit_rect{
    dynamic_cast<QtRoundedEditRectItem*>(qtnode.get())
  };
  QVERIFY(edit_rect);
  QVERIFY(qtnode.get() == edit_rect);
}

void ribi::cmap::QtConceptMapQtNodeTest::qtnode_is_qtroundededitrectitem_with_equal_x()
{
  const auto qtnode = QtNodeFactory().GetTest(1);
  const QtRoundedEditRectItem * edit_rect{
    dynamic_cast<QtRoundedEditRectItem*>(qtnode.get())
  };
  const auto node = GetNode(*qtnode);
  const double node_x = node.GetX();
  const double edit_rect_x = edit_rect->pos().x();
  QVERIFY(std::abs(node_x - edit_rect_x) < 2.0);

}

void ribi::cmap::QtConceptMapQtNodeTest::qtnode_is_qtroundededitrectitem_with_equal_y()
{
  const auto qtnode = QtNodeFactory().GetTest(1);
  const QtRoundedEditRectItem * edit_rect{
    dynamic_cast<QtRoundedEditRectItem*>(qtnode.get())
  };
  const auto node = GetNode(*qtnode);
  const double node_y = node.GetY();
  const double edit_rect_y = edit_rect->pos().y();
  QVERIFY(std::abs(node_y - edit_rect_y) < 2.0);
}

void ribi::cmap::QtConceptMapQtNodeTest::to_str()
{
  const std::unique_ptr<QtNode> qtnode = QtNodeFactory().GetTest(1);
  QVERIFY(!qtnode->ToStr().empty());
}

void ribi::cmap::QtConceptMapQtNodeTest::to_stream()
{
  const std::unique_ptr<QtNode> qtnode = QtNodeFactory().GetTest(1);
  std::stringstream s;
  s << (*qtnode);
  QVERIFY(!s.str().empty());
}
