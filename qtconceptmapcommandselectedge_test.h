#ifndef QTCONCEPTMAP_COMMANDSELECTEDGE_TEST_H
#define QTCONCEPTMAP_COMMANDSELECTEDGE_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtConceptMapCommandSelectEdgeTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:

  //To do/review:


  //Done:

  ///Selecting an absent item fails
  void SelectAbsentItemFails() const noexcept;

  ///Select a QtEdge by name
  void SelectQtEdgeByName() const noexcept;

  ///Select a QtEdge that is connected to center by name
  ///The QtNodes on that QtEdge is disabled and invisible
  void SelectQtEdgeConnectedToCenterByName() const noexcept;

  ///Parse from a string
  void Parse() const noexcept;

  ///Parsing a nonsense string should fails
  void ParseNonsenseFails() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAP_COMMANDSELECTEDGE_TEST_H
