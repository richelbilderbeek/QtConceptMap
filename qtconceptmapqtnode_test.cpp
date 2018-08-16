#include "qtconceptmapqtnode_test.h"
#include "qtconceptmapqtnode.h"

#include <climits>
#include <cmath>
#include <iostream>
#include <sstream>

#include "qtconceptmapqtnodefactory.h"

void ribi::cmap::QtConceptMapQtNodeTest::CenterQtNodeIsCenterNode()
{
  const auto qtnode = QtNodeFactory().GetCenter();
  QVERIFY(IsCenterNode(*qtnode));
}

void ribi::cmap::QtConceptMapQtNodeTest::CountExamples()
{
  const std::unique_ptr<QtNode> qtnode = QtNodeFactory().GetTest(1);
  QVERIFY(ribi::cmap::CountExamples(*qtnode) >= 0);
}

void ribi::cmap::QtConceptMapQtNodeTest::GetExamples()
{
  const std::unique_ptr<QtNode> qtnode = QtNodeFactory().GetTest(1);
  QVERIFY(ribi::cmap::GetExamples(*qtnode).Get().empty());
}

void ribi::cmap::QtConceptMapQtNodeTest::GetName()
{
  const std::unique_ptr<QtNode> qtnode = QtNodeFactory().GetTest(1);
  QVERIFY(!ribi::cmap::GetName(*qtnode).empty());
}

void ribi::cmap::QtConceptMapQtNodeTest::GetRatingComplexity()
{
  const std::unique_ptr<QtNode> qtnode = QtNodeFactory().GetTest(1);
  QVERIFY(ribi::cmap::GetRatingComplexity(*qtnode) >= -1);
}

void ribi::cmap::QtConceptMapQtNodeTest::GetRatingConcreteness()
{
  const std::unique_ptr<QtNode> qtnode = QtNodeFactory().GetTest(1);
  QVERIFY(ribi::cmap::GetRatingConcreteness(*qtnode) >= -1);
}

void ribi::cmap::QtConceptMapQtNodeTest::GetRatingSpecificity()
{
  const std::unique_ptr<QtNode> qtnode = QtNodeFactory().GetTest(1);
  QVERIFY(ribi::cmap::GetRatingSpecificity(*qtnode) >= -1);
}

void ribi::cmap::QtConceptMapQtNodeTest::IsComplex()
{
  const std::unique_ptr<QtNode> qtnode = QtNodeFactory().GetTest(1);
  QVERIFY(ribi::cmap::IsComplex(*qtnode));
}

void ribi::cmap::QtConceptMapQtNodeTest::NormalQtNodeIsNotCenterNode()
{
  const auto qtnode = QtNodeFactory().GetNormal();
  QVERIFY(!IsCenterNode(*qtnode));
}

void ribi::cmap::QtConceptMapQtNodeTest::PressNonsenseIsRejected()
{
  const auto qtnode = QtNodeFactory().GetTest(1);
  QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_F4, Qt::NoModifier);
  qtnode->keyPressEvent(event);
  QVERIFY(!event->isAccepted());
}

void ribi::cmap::QtConceptMapQtNodeTest::QtNodeIsQtRoundedEditRectItem()
{
  const auto qtnode = QtNodeFactory().GetTest(1);
  const QtRoundedEditRectItem * edit_rect{
    dynamic_cast<QtRoundedEditRectItem*>(qtnode.get())
  };
  QVERIFY(edit_rect);
  QVERIFY(qtnode.get() == edit_rect);
}

void ribi::cmap::QtConceptMapQtNodeTest::QtNodeIsQtRoundedEditRectItemWithEqualX()
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

void ribi::cmap::QtConceptMapQtNodeTest::QtNodeIsQtRoundedEditRectItemWithEqualY()
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

void ribi::cmap::QtConceptMapQtNodeTest::ToStr()
{
  const std::unique_ptr<QtNode> qtnode = QtNodeFactory().GetTest(1);
  QVERIFY(!qtnode->ToStr().empty());
}

void ribi::cmap::QtConceptMapQtNodeTest::ToStream()
{
  const std::unique_ptr<QtNode> qtnode = QtNodeFactory().GetTest(1);
  std::stringstream s;
  s << (*qtnode);
  QVERIFY(!s.str().empty());
}

