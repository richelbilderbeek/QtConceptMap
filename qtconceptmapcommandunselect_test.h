#ifndef QTCONCEPTMAP_COMMANDUNSELECT_TEST_H
#define QTCONCEPTMAP_COMMANDUNSELECT_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtCommandUnselectTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:


  //To do/review

  //Done and reviewed
  void UnselectLonelyQtCenterNodeByName() const noexcept;
  void UnselectOneOfTwoQtEdgesByName() const noexcept;
  void UnselectOneOfTwoQtEdgesByNameAndUndo() const noexcept;
  void UnselectOneOfTwoQtNodesByName() const noexcept;
  void UnselectOneOfTwoQtNodesByNameAndUndo() const noexcept;
  void UnselectQtCenterEdgeByName() const noexcept;
  void UnselectQtCenterNodeByName() const noexcept;
  void UnselectTwoQtEdgesByName() const noexcept;
  void UnselectTwoQtEdgesByNameAndUndo() const noexcept;
  void UnselectTwoQtNodesByName() const noexcept;
  void UnselectTwoQtNodesByNameAndUndo() const noexcept;
  void ParseNode() const noexcept;
  void ParseNodeOnEdge() const noexcept;
  void ParseNonsenseFails() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAP_COMMANDUNSELECT_TEST_H

