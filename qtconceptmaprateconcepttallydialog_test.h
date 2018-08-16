#ifndef qtconceptmaprateconcepttallydialog_test_H
#define qtconceptmaprateconcepttallydialog_test_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtConceptMapRateConceptTallyDialogTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void ConstructWithEmptyConceptmap();
  void ConstructWithTestConceptmap();
  void GivesCorrectSuggestions();
  void KeyPresses();
  void MeasureUiFromTestConceptmap();
  void UncheckingDecreasesSuggestion();
  void UserHasNotClickedOkAtConstruction();
};

} //~namespace cmap
} //~namespace ribi

#endif // qtconceptmaprateconcepttallydialog_test_H
