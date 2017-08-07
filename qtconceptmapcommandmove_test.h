#ifndef QTCONCEPTMAP_COMMANDMOVE_TEST_H
#define QTCONCEPTMAP_COMMANDMOVE_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class qtconceptmapcommandmove_test : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void parse() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAP_COMMANDMOVE_TEST_H

