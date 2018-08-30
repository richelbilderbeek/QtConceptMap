#ifndef qtconceptmapqtnode_test_H
#define qtconceptmapqtnode_test_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtNodeTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:

  //Done
  void CenterQtNodeIsCenterNode() const noexcept;
  void CountExamples() const noexcept;
  void GetExamples() const noexcept;
  void GetName() const noexcept;
  void GetRatingComplexity() const noexcept;
  void GetRatingConcreteness() const noexcept;
  void GetRatingSpecificity() const noexcept;
  void IsComplex() const noexcept;
  void NormalQtNodeIsNotCenterNode() const noexcept;
  void PressNonsenseIsRejected() const noexcept;
  void QtNodeIsQtRoundedEditRectItem() const noexcept;
  void QtNodeIsQtRoundedEditRectItemWithEqualX() const noexcept;
  void QtNodeIsQtRoundedEditRectItemWithEqualY() const noexcept;
  void SetRatingComplexity() const noexcept;
  void ToStr() const noexcept;
  void ToStream() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // qtconceptmapqtnode_test_H
