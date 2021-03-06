#ifndef QTCONCEPTMAPHELPER_TEST_H
#define QTCONCEPTMAPHELPER_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class HelperTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void ExtractTheOneSelectedQtEdge() const noexcept;
  void FindQtCenterNode() const noexcept;
  void GetFirstQtEdge() const noexcept;
  void GetQtCenterNode() const noexcept;
  void GetQtEdges() const noexcept;
  void GetQtNodeBrushFunction() const noexcept;
  void GetQtNodeBrushFunctionEdit() const noexcept;
  void GetQtNodeBrushFunctionRate() const noexcept;
  void GetQtNodeBrushFunctionUninitialized() const noexcept;
  void MessUp() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPHELPER_TEST_H
