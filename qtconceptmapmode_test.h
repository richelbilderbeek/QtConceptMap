#ifndef QTCONCEPTMAP_MODE_TEST_H
#define QTCONCEPTMAP_MODE_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class qtconceptmapmode_test : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void str_conversion() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAP_MODE_TEST_H

