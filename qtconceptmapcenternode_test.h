#ifndef QTCONCEPTMAP_CENTERNODE_TEST_H
#define QTCONCEPTMAP_CENTERNODE_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class qtconceptmapcenternode_test : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void all_tests() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAP_CENTERNODE_TEST_H
