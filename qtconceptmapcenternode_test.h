#ifndef QTCONCEPTMAP_CENTERNODE_TEST_H
#define QTCONCEPTMAP_CENTERNODE_TEST_H

#ifdef KEEP_CENTER_QTNODE

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

#endif // KEEP_CENTER_QTNODE

#endif // QTCONCEPTMAP_CENTERNODE_TEST_H
