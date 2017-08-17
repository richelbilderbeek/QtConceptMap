#ifndef QTCONCEPTMAP_COMMANDSELECTNODE_TEST_H
#define QTCONCEPTMAP_COMMANDSELECTNODE_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtConceptMapCommandSelectNodeTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:

  //To do/review:
  ///Select one of two QtNodes by name
  void SelectOneOfTwoQtNodesByName() const noexcept;

  ///Select two QtNode by name
  void SelectTwoQtNodesByName() const noexcept;

  //Done:

  ///Selecting an absent item fails
  void SelectAbsentItemFails() const noexcept;

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

