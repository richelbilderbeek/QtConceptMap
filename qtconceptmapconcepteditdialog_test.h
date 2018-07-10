#ifndef qtconceptmapconcepteditdialog_test_H
#define qtconceptmapconcepteditdialog_test_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtConceptMapConceptEditDialogTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:

  void construction_concept_without_examples();
  void construction_concept_with_one_example();
  void construction_concept_with_two_examples();

  void press_ok_with_changing_examples_should_result_in_changed_concept();
  void press_ok_with_changing_name_should_result_in_changed_concept();
  void press_ok_without_changes_should_result_in_unchanged_concept();
};

} //~namespace cmap
} //~namespace ribi

#endif // qtconceptmapconcepteditdialog_test_H
