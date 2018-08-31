#ifndef QTCONCEPTMAPRATEEXAMPLESDIALOG_TEST_H
#define QTCONCEPTMAPRATEEXAMPLESDIALOG_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtRateExamplesDialogTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void GetRatedExamples() const noexcept;
  void KeyPresses() const noexcept;
  void KeyPressesFromSlots() const noexcept;
  void ThrowsIfNoExamples() const noexcept;
  void UserHasNotClickedOkAtConstruction() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPRATEEXAMPLESDIALOG_TEST_H
