#ifndef qtconceptmapeditconceptdialog_test_H
#define qtconceptmapeditconceptdialog_test_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtEditConceptDialogTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:

  void ClickCancelIsNoted() const noexcept;
  void ClickOkIsNoted() const noexcept;
  void ConstructConceptWithoutExamples() const noexcept;
  void ConstructConceptWithOneExample() const noexcept;
  void ConstructConceptWithTwoExamples() const noexcept;
  void ConstructRelationWithoutExamples() const noexcept;

  void PressOkWithChangingExamplesResultsInChangedConcept() const noexcept;
  void PressOkWithChangingNameResultsInChangedConcept() const noexcept;
  void PressOkWithoutChangesResultsInUnchangedConcept() const noexcept;
  void UserHasNotClickedOkAtConstruction() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // qtconceptmapeditconceptdialog_test_H
