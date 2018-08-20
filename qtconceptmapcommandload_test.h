#ifndef QTCONCEPTMAP_COMMANDLOAD_TEST_H
#define QTCONCEPTMAP_COMMANDLOAD_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtCommandLoadTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void Parse() const noexcept;

  void ParseNonsenseFails() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAP_COMMANDLOAD_TEST_H

