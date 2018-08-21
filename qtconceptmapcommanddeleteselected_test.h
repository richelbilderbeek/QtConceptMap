#ifndef QTCONCEPTMAP_COMMANDDELETESELECTED_TEST_H
#define QTCONCEPTMAP_COMMANDDELETESELECTED_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtCommandDeleteSelectedTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void DeleteSelectedCenterNodeThrows() const noexcept;
  void DeleteSelectedEdge() const noexcept;
  void DeleteSelectedNode() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAP_COMMANDDELETESELECTED_TEST_H

