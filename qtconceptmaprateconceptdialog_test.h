#ifndef QTCONCEPTMAPRATECONCEPTDIALOG_TEST_H
#define QTCONCEPTMAPRATECONCEPTDIALOG_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtConceptMapRateConceptDialogTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void ShowNormalConceptMap();
  void ShowConceptMapWithOnlyCenterNode();
  void UserHasNotPressedOkAtConstruction();

  //compleXity, Concreteness, Specificity
  void XcsAreReadFromQtConceptMap();
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPRATECONCEPTDIALOG_TEST_H
