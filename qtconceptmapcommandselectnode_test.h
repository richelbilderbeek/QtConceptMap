#ifndef QTCONCEPTMAP_COMMANDSELECTNODE_TEST_H
#define QTCONCEPTMAP_COMMANDSELECTNODE_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtCommandSelectNodeTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:

  ///Selecting an absent item fails
  void SelectAbsentItemFails() const noexcept;

  ///Select one of two QtNodes by name
  void SelectOneOfTwoQtNodesByName() const noexcept;

  ///Select one of two QtNodes by name and undo
  void SelectOneOfTwoQtNodesByNameAndUndo() const noexcept;

  ///Selecting a QtNode on a QtEdge fails
  ///(one should select the QtEdge)
  void SelectQtNodeOnQtEdgeFails() const noexcept;

  ///Selecting a QtNode that is already selected fails
  void SelectSelectedQtNodeFails() const noexcept;

  ///Select two QtNodes by name
  void SelectTwoQtNodesByName() const noexcept;

  ///Select two QtNodes by name and undo
  void SelectTwoQtNodesByNameAndUndo() const noexcept;

  ///Select a center QtNode by name
  void SelectQtCenterNodeByName() const noexcept;

  ///Parse from a string
  void Parse() const noexcept;

  ///Parsing a nonsense string should fails
  void ParseNonsenseFails() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAP_COMMANDSELECTNODE_TEST_H

