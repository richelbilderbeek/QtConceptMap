#ifndef qtconceptmapconcepteditdialog_test_H
#define qtconceptmapconcepteditdialog_test_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtConceptMapConceptEditDialogTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:

  void add_should_not_close_the_dialog(); //Brainweaver #118

  void press_ok_with_changing_examples_should_result_in_changed_concept();
  void press_ok_with_changing_name_should_result_in_changed_concept();
  void press_ok_without_changes_should_result_in_unchanged_concept();
};

} //~namespace cmap
} //~namespace ribi

#endif // qtconceptmapconcepteditdialog_test_H
