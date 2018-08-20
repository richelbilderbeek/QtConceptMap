#include "qtconceptmapqtnode_test.h"
#include "qtconceptmapqtnode.h"

#include <climits>
#include <cmath>
#include <iostream>
#include <sstream>

#include "qtconceptmapqtnodefactory.h"

void ribi::cmap::QtNodeTest::CenterQtNodeIsCenterNode()
{
  const auto qtnode = QtNodeFactory().GetCenter();
  QVERIFY(IsCenterNode(*qtnode));
}

void ribi::cmap::QtNodeTest::CountExamples()
{
  const std::unique_ptr<QtNode> qtnode = QtNodeFactory().GetTest(1);
  QVERIFY(ribi::cmap::CountExamples(*qtnode) >= 0);
}

void ribi::cmap::QtNodeTest::GetExamples()
{
  const std::unique_ptr<QtNode> qtnode = QtNodeFactory().GetTest(1);
  QVERIFY(!ribi::cmap::GetExamples(*qtnode).Get().empty());
}

void ribi::cmap::QtNodeTest::GetName()
{
  const std::unique_ptr<QtNode> qtnode = QtNodeFactory().GetTest(1);
  QVERIFY(!ribi::cmap::GetName(*qtnode).empty());
}

void ribi::cmap::QtNodeTest::GetRatingComplexity()
{
  const std::unique_ptr<QtNode> qtnode = QtNodeFactory().GetTest(1);
  QVERIFY(ribi::cmap::GetRatingComplexity(*qtnode) >= -1);
}

void ribi::cmap::QtNodeTest::GetRatingConcreteness()
{
  const std::unique_ptr<QtNode> qtnode = QtNodeFactory().GetTest(1);
  QVERIFY(ribi::cmap::GetRatingConcreteness(*qtnode) >= -1);
}

void ribi::cmap::QtNodeTest::GetRatingSpecificity()
{
  const std::unique_ptr<QtNode> qtnode = QtNodeFactory().GetTest(1);
  QVERIFY(ribi::cmap::GetRatingSpecificity(*qtnode) >= -1);
}

void ribi::cmap::QtNodeTest::IsComplex()
{
  const std::unique_ptr<QtNode> qtnode = QtNodeFactory().GetTest(1);
  QVERIFY(ribi::cmap::IsComplex(*qtnode));
}

void ribi::cmap::QtNodeTest::NormalQtNodeIsNotCenterNode()
{
  const auto qtnode = QtNodeFactory().GetNormal();
  QVERIFY(!IsCenterNode(*qtnode));
}

void ribi::cmap::QtNodeTest::PressNonsenseIsRejected()
{
  const auto qtnode = QtNodeFactory().GetTest(1);
  QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_F4, Qt::NoModifier);
  qtnode->keyPressEvent(event);
  QVERIFY(!event->isAccepted());
}

void ribi::cmap::QtNodeTest::QtNodeIsQtRoundedEditRectItem()
{
  const auto qtnode = QtNodeFactory().GetTest(1);
  const QtRoundedEditRectItem * edit_rect{
    dynamic_cast<QtRoundedEditRectItem*>(qtnode.get())
  };
  QVERIFY(edit_rect);
  QVERIFY(qtnode.get() == edit_rect);
}

void ribi::cmap::QtNodeTest::QtNodeIsQtRoundedEditRectItemWithEqualX()
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

void ribi::cmap::QtNodeTest::QtNodeIsQtRoundedEditRectItemWithEqualY()
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

void ribi::cmap::QtNodeTest::ToStr()
{
  const std::unique_ptr<QtNode> qtnode = QtNodeFactory().GetTest(1);
  QVERIFY(!qtnode->ToStr().empty());
}

void ribi::cmap::QtNodeTest::ToStream()
{
  const std::unique_ptr<QtNode> qtnode = QtNodeFactory().GetTest(1);
  std::stringstream s;
  s << (*qtnode);
  QVERIFY(!s.str().empty());
}

