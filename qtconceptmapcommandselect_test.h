#ifndef QTCONCEPTMAP_COMMANDSELECT_TEST_H
#define QTCONCEPTMAP_COMMANDSELECT_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class qtconceptmapcommandselect_test : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void parse() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAP_COMMANDSELECT_TEST_H

