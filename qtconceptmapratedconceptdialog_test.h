#ifndef QTCONCEPTMAPRATEDCONCEPTDIALOG_TEST_H
#define QTCONCEPTMAPRATEDCONCEPTDIALOG_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtRatedConceptDialogTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void show_assessor();
  void show_student();
  void show_example();
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPRATEDCONCEPTDIALOG_TEST_H
