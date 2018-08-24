#ifndef QTCONCEPTMAP_COMMANDMOVENODE_TEST_H
#define QTCONCEPTMAP_COMMANDMOVENODE_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtCommandMoveNodeTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:

  //To do/review:

  //Done:

  ///Does the Command throw when trying to move an absent item?
  void MoveAbsentItemByNameFails() const noexcept;

  ///Does the Command move a QtNode?
  void MoveCenterQtNodeByNameFails() const noexcept;

  ///Does the Command move a QtEdge connected to a center QtNode, in a QtConceptMap with two QtEdges?
  void MoveEdgeConnectedToCenterFails() const noexcept;

  ///Does the Command move a QtEdge, in a QtConceptMap with two QtEdges?
  void MoveEdgeFails() const noexcept;

  ///Does the Command move a QtNode when search a QtNode by its name?
  void MoveQtNodeWithName() const noexcept;

  ///Does the Command move a QtNode when searching a QtNode by its position?
  void MoveQtNodeWithPosition() const noexcept;

  ///Can the Command be created by parsing a string?
  void Parse() const noexcept;

  ///Parsing a string to move a center node returns nullptr
  void ParseMoveAbsentNodeReturnsNullptr() const noexcept;

  ///Parsing a string to move a center node returns nullptr
  void ParseMoveCenterNodeReturnsNullptr() const noexcept;

  ///The Command cannot created by parsing nonsense
  void ParseNonsenseFails() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAP_COMMANDMOVENODE_TEST_H

