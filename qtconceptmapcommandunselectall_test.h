#ifndef QTCONCEPTMAP_COMMANDUNSELECTALL_TEST_H
#define QTCONCEPTMAP_COMMANDUNSELECTALL_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtConceptMapCommandUnselectAllTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:

  //TODO
  ///UnselectAll one of two QtEdges by name
  void UnselectAllOneOfTwoQtEdgesByName() const noexcept;

  //DONE
  ///UnselectAlling an absent item fails
  void UnselectAllAbsentItemFails() const noexcept;


  ///UnselectAll one of two QtEdges by name and undo
  void UnselectAllOneOfTwoQtEdgesByNameAndUndo() const noexcept;

  ///UnselectAll one of two QtNodes by name
  void UnselectAllOneOfTwoQtNodesByName() const noexcept;

  ///UnselectAll one of two QtNodes by name and undo
  void UnselectAllOneOfTwoQtNodesByNameAndUndo() const noexcept;

  ///UnselectAll a center QtEdge by name
  void UnselectAllQtCenterEdgeByName() const noexcept;

  ///UnselectAll a center QtNode by name
  void UnselectAllQtCenterNodeByName() const noexcept;

  ///UnselectAll two QtEdges by name
  void UnselectAllTwoQtEdgesByName() const noexcept;

  ///UnselectAll two QtEdges by name and undo
  void UnselectAllTwoQtEdgesByNameAndUndo() const noexcept;

  ///UnselectAll two QtNodes by name
  void UnselectAllTwoQtNodesByName() const noexcept;

  ///UnselectAll two QtNodes by name and undo
  void UnselectAllTwoQtNodesByNameAndUndo() const noexcept;

  ///Parse from a string
  void Parse() const noexcept;

  ///Parsing a nonsense string should fails
  void ParseNonsenseFails() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAP_COMMANDUNSELECTALL_TEST_H

