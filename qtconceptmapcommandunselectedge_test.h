#ifndef QTCONCEPTMAP_COMMANDUNSELECTEDGE_TEST_H
#define QTCONCEPTMAP_COMMANDUNSELECTEDGE_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtCommandUnselectEdgeTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:

  //To do/review:
  ///Unselecting an absent item fails
  void UnselectAbsentItemFails() const noexcept;

  ///Unselecting a non-selected item fails
  void UnselectNonSelectedItemFails() const noexcept;

  ///Unselect one of two QtEdges by name
  void UnselectOneOfTwoQtEdgesByName() const noexcept;

  ///Unselect one of two QtEdges by name and undo
  void UnselectOneOfTwoQtEdgesByNameAndUndo() const noexcept;

  ///Unselect two QtEdges by name
  void UnselectTwoQtEdgesByName() const noexcept;

  ///Unselect two QtEdges by name and undo
  void UnselectTwoQtEdgesByNameAndUndo() const noexcept;

  ///Unselect a center QtEdge by name
  void UnselectQtCenterEdgeByName() const noexcept;

  ///Parse from a string
  void Parse() const noexcept;

  ///Parsing a nonsense string should fails
  void ParseNonsenseFails() const noexcept;

  //Done:
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAP_COMMANDUNSELECTEDGE_TEST_H

