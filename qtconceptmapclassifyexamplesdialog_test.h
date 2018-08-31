#ifndef QTCONCEPTMAPCLASSIFYEXAMPLESDIALOG_TEST_H
#define QTCONCEPTMAPCLASSIFYEXAMPLESDIALOG_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtClassifyExamplesDialogTest : public QObject
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

#endif // QTCONCEPTMAPCLASSIFYEXAMPLESDIALOG_TEST_H
