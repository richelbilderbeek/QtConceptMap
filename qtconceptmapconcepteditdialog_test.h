#ifndef qtconceptmapconcepteditdialog_test_H
#define qtconceptmapconcepteditdialog_test_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtConceptEditDialogTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:

  void ConstructConceptWithoutExamples() const noexcept;
  void ConstructConceptWithOneExample() const noexcept;
  void ConstructConceptWithTwoExamples() const noexcept;
  void ConstructRelationWithoutExamples() const noexcept;

  void PressOkWithChangingExamplesResultsInChangedConcept() const noexcept;
  void PressOkWithChangingNameResultsInChangedConcept() const noexcept;
  void PressOkWithoutChangesResultsInUnchangedConcept() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // qtconceptmapconcepteditdialog_test_H
