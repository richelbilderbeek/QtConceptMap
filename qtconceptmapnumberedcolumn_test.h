#ifndef QTCONCEPTMAPNUMBEREDCOLUMN_TEST_H
#define QTCONCEPTMAPNUMBEREDCOLUMN_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtNumberedColumnTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void Construction();
  void EmptyRowIsDeleted();
  void RandomKeyPress();
  void TabChangesFocus();
};

} //~namespace cmap
} //~namespace ribi


#endif // QTCONCEPTMAPNUMBEREDCOLUMN_TEST_H
