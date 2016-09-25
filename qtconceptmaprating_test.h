#ifndef QTCONCEPTMAPRATING_TEST_H
#define QTCONCEPTMAPRATING_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class qtconceptmaprating_test : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void all_tests();
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPRATING_TEST_H
