#ifndef QTCONCEPTMAP_COMMANDSELECTEDGE_TEST_H
#define QTCONCEPTMAP_COMMANDSELECTEDGE_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtCommandSelectEdgeTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:

  //To do/review:

  ///Select a QtEdge by name and undo
  void SelectQtEdgeByNameAndUndo() const noexcept;

  //Done:

  ///Selecting an absent item fails
  void SelectAbsentItemFails() const noexcept;

  ///Select a QtEdge by name
  void SelectQtEdgeByName() const noexcept;

  ///Select a QtEdge that is connected to center by name
  ///The QtNodes on that QtEdge is disabled and invisible
  void SelectQtEdgeConnectedToCenterByName() const noexcept;

  ///Selecting a QtEdge that is already selected should throw
  void SelectSelectedQtEdgeFails() const noexcept;

  ///Select two QtEdges by name
  void SelectTwoQtEdgesByName() const noexcept;

  ///Parse from a string
  void Parse() const noexcept;

  ///Parsing a nonsense string should fails
  void ParseNonsenseFails() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAP_COMMANDSELECTEDGE_TEST_H

