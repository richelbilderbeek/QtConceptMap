#ifndef QTCONCEPTMAP_COMMANDMOVE_TEST_H
#define QTCONCEPTMAP_COMMANDMOVE_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtConceptMapCommandMoveTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:

  ///Does the Command throw when trying to move an absent item?
  void MoveAbsentItemFails() const noexcept;

  ///Does the Command move a QtNode?
  void MoveCenterQtNodeFails() const noexcept;

  ///Does the Command move a QtEdge?
  void MoveQtEdge() const noexcept;

  ///Does the Command move a QtEdge connected to a center QtNode?
  void MoveQtEdgeConnectedToCenterQtNode() const noexcept;

  ///Does the Command move a QtNode?
  void MoveQtNode() const noexcept;

  ///Can the Command be created by parsing a string?
  void Parse() const noexcept;

  ///The Command cannot created by parsing nonsense
  void ParseNonsenseFails() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAP_COMMANDMOVE_TEST_H

