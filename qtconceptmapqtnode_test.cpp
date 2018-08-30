#include "qtconceptmapqtnode_test.h"
#include "qtconceptmapqtnode.h"

#include <cassert>
#include <climits>
#include <cmath>
#include <iostream>
#include <sstream>

#include "qtconceptmapqtnodefactory.h"

void ribi::cmap::QtNodeTest::CenterQtNodeIsCenterNode() const noexcept
{
  const auto qtnode = QtNodeFactory().GetCenter();
  QVERIFY(IsCenterNode(*qtnode));
}

void ribi::cmap::QtNodeTest::CountExamples() const noexcept
{
  const std::unique_ptr<QtNode> qtnode = QtNodeFactory().GetTest(1);
  QVERIFY(ribi::cmap::CountExamples(*qtnode) >= 0);
}

void ribi::cmap::QtNodeTest::GetExamples() const noexcept
{
  const std::unique_ptr<QtNode> qtnode = QtNodeFactory().GetTest(1);
  QVERIFY(!ribi::cmap::GetExamples(*qtnode).Get().empty());
}

void ribi::cmap::QtNodeTest::GetName() const noexcept
{
  const std::unique_ptr<QtNode> qtnode = QtNodeFactory().GetTest(1);
  QVERIFY(!ribi::cmap::GetName(*qtnode).empty());
}

void ribi::cmap::QtNodeTest::GetRatingComplexity() const noexcept
{
  const std::unique_ptr<QtNode> qtnode = QtNodeFactory().GetTest(1);
  QVERIFY(ribi::cmap::GetRatingComplexity(*qtnode) >= -1);
}

void ribi::cmap::QtNodeTest::GetRatingConcreteness() const noexcept
{
  const std::unique_ptr<QtNode> qtnode = QtNodeFactory().GetTest(1);
  QVERIFY(ribi::cmap::GetRatingConcreteness(*qtnode) >= -1);
}

void ribi::cmap::QtNodeTest::GetRatingSpecificity() const noexcept
{
  const std::unique_ptr<QtNode> qtnode = QtNodeFactory().GetTest(1);
  QVERIFY(ribi::cmap::GetRatingSpecificity(*qtnode) >= -1);
}

void ribi::cmap::QtNodeTest::IsComplex() const noexcept
{
  const std::unique_ptr<QtNode> qtnode = QtNodeFactory().GetTest(1);
  QVERIFY(ribi::cmap::IsComplex(*qtnode));
}

void ribi::cmap::QtNodeTest::NormalQtNodeIsNotCenterNode() const noexcept
{
  const auto qtnode = QtNodeFactory().GetNormal();
  QVERIFY(!IsCenterNode(*qtnode));
}

void ribi::cmap::QtNodeTest::PressNonsenseIsRejected() const noexcept
{
  const auto qtnode = QtNodeFactory().GetTest(1);
  QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_F4, Qt::NoModifier);
  qtnode->keyPressEvent(event);
  QVERIFY(!event->isAccepted());
}

void ribi::cmap::QtNodeTest::QtNodeIsQtRoundedEditRectItem() const noexcept
{
  const auto qtnode = QtNodeFactory().GetTest(1);
  const QtRoundedEditRectItem * edit_rect{
    dynamic_cast<QtRoundedEditRectItem*>(qtnode.get())
  };
  QVERIFY(edit_rect);
  QVERIFY(qtnode.get() == edit_rect);
}

void ribi::cmap::QtNodeTest::QtNodeIsQtRoundedEditRectItemWithEqualX() const noexcept
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

void ribi::cmap::QtNodeTest::QtNodeIsQtRoundedEditRectItemWithEqualY() const noexcept
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

void ribi::cmap::QtNodeTest::SetRatingComplexity() const noexcept
{
  QtNode qtnode;
  //Valid rating
  {
    const int rating{1};
    ::ribi::cmap::SetRatingComplexity(qtnode, rating);
    QVERIFY(rating == ::ribi::cmap::GetRatingComplexity(qtnode));
  }
  //Invalid rating must throw
  {
    const int rating{-123};
    try
    {
      ::ribi::cmap::SetRatingComplexity(qtnode, rating);
      assert(!"Should not get here");
    }
    catch (const std::exception&)
    {
      QVERIFY("Should get here");
    }
  }
}

void ribi::cmap::QtNodeTest::ToStr() const noexcept
{
  const std::unique_ptr<QtNode> qtnode = QtNodeFactory().GetTest(1);
  QVERIFY(!qtnode->ToStr().empty());
}

void ribi::cmap::QtNodeTest::ToStream() const noexcept
{
  const std::unique_ptr<QtNode> qtnode = QtNodeFactory().GetTest(1);
  std::stringstream s;
  s << (*qtnode);
  QVERIFY(!s.str().empty());
}

