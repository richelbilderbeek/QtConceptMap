#ifndef QTCONCEPTMAP_COMMANDUNSELECTNODE_TEST_H
#define QTCONCEPTMAP_COMMANDUNSELECTNODE_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtCommandUnselectNodeTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:

  //To do/review:


  //Done:

  void UnselectAbsentItemFails() const noexcept;
  void UnselectLonelyQtCenterNodeByName() const noexcept;
  void UnselectOneOfTwoQtNodesByName() const noexcept;
  void UnselectOneOfTwoQtNodesByNameAndUndo() const noexcept;
  void UnselectQtCenterNodeByName() const noexcept;
  void UnselectQtNodeOnQtEdgeFails() const noexcept; // (one should select the QtEdge)
  void UnselectTwoQtNodesByName() const noexcept;
  void UnselectTwoQtNodesByNameAndUndo() const noexcept;
  void Parse() const noexcept;
  void ParseNonsenseFails() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAP_COMMANDUNSELECTNODE_TEST_H

