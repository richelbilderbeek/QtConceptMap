#ifndef QTCONCEPTMAP_COMMANDUNSELECTNODE_TEST_H
#define QTCONCEPTMAP_COMMANDUNSELECTNODE_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtConceptMapCommandUnselectNodeTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:

  //To do/review:


  //Done:

  ///Unselecting an absent item fails
  void UnselectAbsentItemFails() const noexcept;

  ///Unselect a lonely center QtNode by name
  void UnselectLonelyQtCenterNodeByName() const noexcept;

  ///Unselect one of two QtNodes by name
  void UnselectOneOfTwoQtNodesByName() const noexcept;

  ///Unselect one of two QtNodes by name and undo
  void UnselectOneOfTwoQtNodesByNameAndUndo() const noexcept;


  ///Unselect a center QtNode by name
  void UnselectQtCenterNodeByName() const noexcept;

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

#endif // QTCONCEPTMAP_COMMANDUNSELECTNODE_TEST_H

