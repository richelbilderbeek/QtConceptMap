#ifndef QTCONCEPTMAPRATECONCEPTDIALOG_TEST_H
#define QTCONCEPTMAPRATECONCEPTDIALOG_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtRateConceptDialogTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void EscapeClosesDialog() const noexcept;
  void PressCancelIsKnown() const noexcept;
  void PressOkIsStored() const noexcept;
  void ShowConceptMapWithOnlyCenterNode() const noexcept;
  void ShowNormalConceptMap() const noexcept;
  void Write() const noexcept;
  void XcsAreReadFromQtConceptMap() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPRATECONCEPTDIALOG_TEST_H
