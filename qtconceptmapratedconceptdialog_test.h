#ifndef QTCONCEPTMAPRATEDCONCEPTDIALOG_TEST_H
#define QTCONCEPTMAPRATEDCONCEPTDIALOG_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class qtconceptmapratedconceptdialog_test : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void show_normal();
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPRATEDCONCEPTDIALOG_TEST_H
