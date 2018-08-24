#ifndef QTCONCEPTMAP_COMMANDUNSELECTALL_TEST_H
#define QTCONCEPTMAP_COMMANDUNSELECTALL_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtCommandUnselectAllTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void Parse() const noexcept;
  void ParseNonsenseFails() const noexcept;
  void UnselectAllAbsentItemFails() const noexcept;
  void UnselectAllOneOfTwoQtEdgesByName() const noexcept;
  void UnselectAllOneOfTwoQtEdgesByNameAndUndo() const noexcept;
  void UnselectAllOneOfTwoQtNodesByName() const noexcept;
  void UnselectAllOneOfTwoQtNodesByNameAndUndo() const noexcept;
  void UnselectAllQtCenterEdgeByName() const noexcept;
  void UnselectAllQtCenterNodeByName() const noexcept;
  void UnselectAllTwoQtEdgesByName() const noexcept;
  void UnselectAllTwoQtEdgesByNameAndUndo() const noexcept;
  void UnselectAllTwoQtNodesByName() const noexcept;
  void UnselectAllTwoQtNodesByNameAndUndo() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAP_COMMANDUNSELECTALL_TEST_H

