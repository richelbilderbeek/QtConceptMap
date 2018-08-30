#ifndef qtconceptmaprateexamplesdialog_test_H
#define qtconceptmaprateexamplesdialog_test_H

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

#endif // qtconceptmaprateexamplesdialog_test_H
