#ifndef QTCONCEPTMAP_COMMANDMOVEEDGE_TEST_H
#define QTCONCEPTMAP_COMMANDMOVEEDGE_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtCommandMoveEdgeTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:

  //To do/review:
  void MoveQtEdgeInEditModeWorks() const noexcept;
  void MoveQtEdgeInRateModeFails() const noexcept;
  void MoveQtEdgeInUninitializedModeFails() const noexcept;

  //Done:

  ///Does the Command throw when trying to move an absent item?
  void MoveAbsentItemByNameFails() const noexcept;

  ///Does the Command move a QtEdge, in a QtConceptMap with two QtEdges?
  void MoveOneOfTwoQtEdgesByName() const noexcept;

  ///Does the Command move a QtEdge connected to a center QtNode, in a QtConceptMap with two QtEdges?
  void MoveOneOfTwoQtEdgesConnectedToCenterQtNodeByName() const noexcept;

  ///Does the Command move a QtEdge, in a QtConceptMap with one QtEdge?
  void MoveOnlyQtEdgeByName() const noexcept;

  ///Move a QtEdge, then undo
  void MoveOnlyQtEdgeByNameAndUndo() const noexcept;

  ///Does the Command move a QtEdge connected to a center QtNode, in a QtConceptMap one QtEdge?
  void MoveOnlyQtEdgeConnectedToCenterQtNode() const noexcept;


  ///Can the Command be created by parsing a string?
  void Parse() const noexcept;

  ///The Command cannot created by parsing nonsense
  void ParseNonsenseFails() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAP_COMMANDMOVEEDGE_TEST_H

