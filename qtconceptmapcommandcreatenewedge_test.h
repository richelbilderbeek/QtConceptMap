#ifndef QTCONCEPTMAP_COMMANDCREATENEWEDGE_TEST_H
#define QTCONCEPTMAP_COMMANDCREATENEWEDGE_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtConceptMapCommandCreateNewEdgeTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void parse() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAP_COMMANDCREATENEWEDGE_TEST_H

