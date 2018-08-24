#ifndef QTCONCEPTMAPRATECONCEPTDIALOG_TEST_H
#define QTCONCEPTMAPRATECONCEPTDIALOG_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtRateConceptDialogTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void EscapeClosesDialog() const;
  void ShowNormalConceptMap() const;
  void ShowConceptMapWithOnlyCenterNode() const;

  //compleXity, Concreteness, Specificity
  void XcsAreReadFromQtConceptMap() const;

  void Write() const;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPRATECONCEPTDIALOG_TEST_H
