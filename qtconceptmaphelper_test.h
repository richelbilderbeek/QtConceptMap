#ifndef QTCONCEPTMAPHELPER_TEST_H
#define QTCONCEPTMAPHELPER_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtConceptMapHelperTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void ExtractTheOneSelectedQtEdge() const noexcept;
  void GetQtCenterNode() const noexcept;
  void MessUp() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPHELPER_TEST_H
