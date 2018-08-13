#ifndef QTCONCEPTMAP_COMMANDCREATENEWNODE_TEST_H
#define QTCONCEPTMAP_COMMANDCREATENEWNODE_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtConceptMapCommandCreateNewNodeTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void CreateNewCenterNode() const noexcept;
  void CreateNewNode() const noexcept;
  void Parse() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAP_COMMANDCREATENEWNODE_TEST_H

