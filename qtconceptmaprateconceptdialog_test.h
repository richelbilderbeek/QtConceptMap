#ifndef QTCONCEPTMAPRATECONCEPTDIALOG_TEST_H
#define QTCONCEPTMAPRATECONCEPTDIALOG_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtConceptMapRateConceptDialogTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void EscapeClosesDialog() const;
  void OkClosesDialog() const;
  void OkStoresOkayness() const;
  void ShowNormalConceptMap() const;
  void ShowConceptMapWithOnlyCenterNode() const;
  void TallyRelevanciesCloses() const;
  void TallyRelevanciesPopsUp() const;
  void UserHasNotPressedOkAtConstruction() const;

  //compleXity, Concreteness, Specificity
  void XcsAreReadFromQtConceptMap() const;

  void Write() const;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPRATECONCEPTDIALOG_TEST_H
