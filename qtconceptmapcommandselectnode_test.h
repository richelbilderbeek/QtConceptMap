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


  //Done:

  ///Selecting an absent item fails
  void SelectAbsentItemFails() const noexcept;

  ///Select a center QtNode by name
  void SelectQtCenterNodeByName() const noexcept;

  ///Select a QtNode by name
  void SelectQtNodeByName() const noexcept;

  ///Parse from a string
  void Parse() const noexcept;

  ///Parsing a nonsense string should fails
  void ParseNonsenseFails() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAP_COMMANDSELECTNODE_TEST_H

