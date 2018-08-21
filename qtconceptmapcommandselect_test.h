#ifndef QTCONCEPTMAP_COMMANDSELECT_TEST_H
#define QTCONCEPTMAP_COMMANDSELECT_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtCommandSelectTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:

  //To do/review:


  //Done:

  ///Select a center QtNode by name
  void SelectQtCenterNodeByName() const noexcept;

  ///Select a QtEdge by name
  void SelectQtEdgeByName() const noexcept;

  ///Select a QtEdge that is connected to center by name
  ///The QtNodes on that QtEdge is disabled and invisible
  void SelectQtEdgeConnectedToCenterByName() const noexcept;

  ///Select a QtNode by name
  void SelectQtNodeByName() const noexcept;

  ///Parse from a string, search for a center node
  void ParseCenterNode() const noexcept;

  ///Parse from a string, search for an edge
  void ParseEdge() const noexcept;

  ///Parse from a string, search for a node
  void ParseNode() const noexcept;

  ///Parsing a nonsense string should fails
  void ParseNonsenseFails() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAP_COMMANDSELECT_TEST_H

