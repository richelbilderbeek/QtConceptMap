#ifndef QTCONCEPTMAP_COMMANDUNSELECT_TEST_H
#define QTCONCEPTMAP_COMMANDUNSELECT_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtConceptMapCommandUnselectTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:


  //To do/review

  //Done and reviewed
  ///Unselecting an absent item fails
  void UnselectAbsentItemFails() const noexcept;

  ///Unselect a lonely QtNode by name
  void UnselectLonelyQtCenterNodeByName() const noexcept;

  ///Unselect one of two QtEdges by name
  void UnselectOneOfTwoQtEdgesByName() const noexcept;

  ///Unselect one of two QtEdges by name and undo
  void UnselectOneOfTwoQtEdgesByNameAndUndo() const noexcept;

  ///Unselect one of two QtNodes by name
  void UnselectOneOfTwoQtNodesByName() const noexcept;

  ///Unselect one of two QtNodes by name and undo
  void UnselectOneOfTwoQtNodesByNameAndUndo() const noexcept;

  ///Unselect a center QtEdge by name
  void UnselectQtCenterEdgeByName() const noexcept;

  ///Unselect a center QtNode by name
  void UnselectQtCenterNodeByName() const noexcept;

  ///Unselect two QtEdges by name
  void UnselectTwoQtEdgesByName() const noexcept;

  ///Unselect two QtEdges by name and undo
  void UnselectTwoQtEdgesByNameAndUndo() const noexcept;

  ///Unselect two QtNodes by name
  void UnselectTwoQtNodesByName() const noexcept;

  ///Unselect two QtNodes by name and undo
  void UnselectTwoQtNodesByNameAndUndo() const noexcept;

  ///Parse from a string
  void Parse() const noexcept;

  ///Parsing a nonsense string should fails
  void ParseNonsenseFails() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAP_COMMANDUNSELECT_TEST_H

