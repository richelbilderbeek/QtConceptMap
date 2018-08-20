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
  void AllTests() const noexcept;
  void ChangeFocus() const noexcept;
  void ConstructAbuse() const noexcept;
  void GetNodeMustBeCorrect() const noexcept;
  void OperatorEquals() const noexcept;
  void NodeMustHaveDottedBlackLineWhenSelected() const noexcept; //#127
  void NodeMustHaveWhiteBorderWhenNotSelected() const noexcept; //#127
  void SetExamples() const noexcept;
  void SetIsComplex() const noexcept;
  void ShowBoundingRect() const noexcept;
  void ToStr() const noexcept;
  void ToStream() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // qtconceptmapqtedge_test_H
