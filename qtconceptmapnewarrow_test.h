#ifndef QTCONCEPTMAPNEWARROW_TEST_H
#define QTCONCEPTMAPNEWARROW_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtNewArrowTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void CancelActiveArrow() const noexcept;
  void CreateNewQtEdge() const noexcept;
  void StartSetsSourceQtNode() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPNEWARROW_TEST_H
