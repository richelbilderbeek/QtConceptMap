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
  void CenterQtNodeIsCenterNode();
  void CountExamples();
  void GetExamples();
  void GetName();
  void GetRatingComplexity();
  void GetRatingConcreteness();
  void GetRatingSpecificity();
  void IsComplex();
  void NormalQtNodeIsNotCenterNode();
  void PressNonsenseIsRejected();
  void QtNodeIsQtRoundedEditRectItem();
  void QtNodeIsQtRoundedEditRectItemWithEqualX();
  void QtNodeIsQtRoundedEditRectItemWithEqualY();
  void ToStr();
  void ToStream();
};

} //~namespace cmap
} //~namespace ribi

#endif // qtconceptmapqtnode_test_H
