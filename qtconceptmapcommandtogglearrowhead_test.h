#ifndef QTCONCEPTMAP_COMMANDTOGGLEARROWHEAD_TEST_H
#define QTCONCEPTMAP_COMMANDTOGGLEARROWHEAD_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtCommandToggleArrowHeadTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:

  void ToggleAbsentEdgeShouldFail() const noexcept;

  void ToggleEdge() const noexcept;

  void ToggleEdgeConnectedToCenterNode() const noexcept;


  void Parse() const noexcept;

  void ParseNonsenseFails() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAP_COMMANDTOGGLEARROWHEAD_TEST_H

