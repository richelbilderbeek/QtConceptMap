#ifndef qtconceptmaprateconcepttallydialog_test_H
#define qtconceptmaprateconcepttallydialog_test_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtRateConceptTallyDialogTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void ConstructWithEmptyConceptmap() const noexcept;
  void ConstructWithTestConceptmap() const noexcept;
  void GivesCorrectSuggestions() const noexcept;
  void KeyPresses() const noexcept;
  void MeasureUiFromTestConceptmap() const noexcept;
  void UncheckingDecreasesSuggestion() const noexcept;
  void UserHasNotClickedOkAtConstruction() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // qtconceptmaprateconcepttallydialog_test_H
