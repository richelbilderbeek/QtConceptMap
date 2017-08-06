#ifndef QTCONCEPTMAP_COMMANDSAVE_TEST_H
#define QTCONCEPTMAP_COMMANDSAVE_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class qtconceptmapcommandsave_test : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void parse() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAP_COMMANDSAVE_TEST_H
