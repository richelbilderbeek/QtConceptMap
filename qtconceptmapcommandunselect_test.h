#ifndef QTCONCEPTMAP_COMMANDUNSELECT_TEST_H
#define QTCONCEPTMAP_COMMANDUNSELECT_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtConceptMapCommandUnselectTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void parse() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAP_COMMANDUNSELECT_TEST_H

