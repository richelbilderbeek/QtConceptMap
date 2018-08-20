#ifndef qtconceptmapqtedge_test_H
#define qtconceptmapqtedge_test_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtEdgeTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:

  //To do/review:

  //Done:
  void AllTests();
  void ChangeFocus();
  void GetNodeMustBeCorrect();
  void NodeMustHaveDottedBlackLineWhenSelected(); //#127
  void NodeMustHaveWhiteBorderWhenNotSelected(); //#127
  void ShowBoundingRect();
  void ToStr();
  void ToStream();
};

} //~namespace cmap
} //~namespace ribi

#endif // qtconceptmapqtedge_test_H
