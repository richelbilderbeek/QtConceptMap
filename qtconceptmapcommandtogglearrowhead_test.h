#ifndef QTCONCEPTMAP_COMMANDTOGGLEARROWHEAD_TEST_H
#define QTCONCEPTMAP_COMMANDTOGGLEARROWHEAD_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtConceptMapCommandToggleArrowHeadTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:

  void Parse() const noexcept;

  void ParseNonsenseFails() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAP_COMMANDTOGGLEARROWHEAD_TEST_H

