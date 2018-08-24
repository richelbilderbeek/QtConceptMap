#ifndef QTCONCEPTMAP_COMMAND_TEST_H
#define QTCONCEPTMAP_COMMAND_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtCommandTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:

  void AllGettersWork() const noexcept;
  void AllConstGettersWork() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAP_COMMAND_TEST_H

