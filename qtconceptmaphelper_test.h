#ifndef QTCONCEPTMAPHELPER_TEST_H
#define QTCONCEPTMAPHELPER_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class qtconceptmaphelper_test : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void count_qt_nodes();
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPHELPER_TEST_H
