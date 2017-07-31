#ifndef QTCONCEPTMAP_COMMANDSETMODE_TEST_H
#define QTCONCEPTMAP_COMMANDSETMODE_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class qtconceptmapcommandsetmode_test : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void parse() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAP_COMMANDSETMODE_TEST_H

