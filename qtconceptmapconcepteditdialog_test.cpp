#include "qtconceptmapconcepteditdialog_test.h"
#include "qtconceptmapconcepteditdialog.h"


#include <QKeyEvent>
#include <QObjectList>

#include "conceptmapcompetency.h"
#include "conceptmapexample.h"
#include "conceptmapexamplefactory.h"
#include "conceptmapexamplesfactory.h"
#include "conceptmapexamples.h"
#include "conceptmaphelper.h"
#include "conceptmapconcept.h"
#include "conceptmapconceptfactory.h"
#include "qtconceptmapcompetency.h"
#include "ui_qtconceptmapconcepteditdialog.h"


using namespace ribi::cmap;

void ribi::cmap::QtConceptMapConceptEditDialogTest
  ::construction_concept_with_one_example()
{
  QtConceptMapConceptEditDialog d(
    ConceptFactory().Get1(),
    QtConceptMapConceptEditDialog::EditType::concept
  );
  d.show();
}

void ribi::cmap::QtConceptMapConceptEditDialogTest
  ::construction_concept_with_two_examples()
{
  QtConceptMapConceptEditDialog d(
    ConceptFactory().Get2(),
    QtConceptMapConceptEditDialog::EditType::concept
  );
  d.show();
}


void ribi::cmap::QtConceptMapConceptEditDialogTest
  ::construction_concept_without_examples()
{
  QtConceptMapConceptEditDialog d(
    ConceptFactory().Get0(),
    QtConceptMapConceptEditDialog::EditType::concept
  );
  d.show();
}

void ribi::cmap::QtConceptMapConceptEditDialogTest
  ::press_ok_with_changing_examples_should_result_in_changed_concept()
{
  //Assume reading in a concept and clicking OK after adding an example
  for (const auto concept: ConceptFactory().GetTests())
  {
    QtConceptMapConceptEditDialog d(
      concept,
      QtConceptMapConceptEditDialog::EditType::concept
    );
    QVERIFY(d.ui->edit_text->toPlainText().isEmpty());
    d.ui->edit_text->setPlainText("TO BE ADDED EXAMPLE");
    d.on_button_add_clicked(); //Adds text
    const Concept after(d.GetConcept());
    QVERIFY(concept.GetName() == after.GetName());
    QVERIFY(CollectExamplesTexts(concept) != CollectExamplesTexts(after));
  }
}



void ribi::cmap::QtConceptMapConceptEditDialogTest
  ::press_ok_with_changing_name_should_result_in_changed_concept()
{
  //Assume reading in a concept and clicking OK after modification of the name does modify concept
  for (const auto concept: ConceptFactory().GetTests())
  {
    QtConceptMapConceptEditDialog d(
      concept,
      QtConceptMapConceptEditDialog::EditType::concept
    );
    d.ui->edit_concept->setPlainText(d.ui->edit_concept->toPlainText() + "MODIFICATION");
    const Concept after(d.GetConcept());
    QVERIFY(concept.GetName() != after.GetName());
    QVERIFY(CollectExamplesTexts(concept) == CollectExamplesTexts(after));
  }
}


void ribi::cmap::QtConceptMapConceptEditDialogTest
  ::press_ok_without_changes_should_result_in_unchanged_concept()
{
  //Assume reading in a concept and clicking OK without modification does not modify anything
  for (const auto concept: ConceptFactory().GetTests())
  {
    QtConceptMapConceptEditDialog d(
      concept,
      QtConceptMapConceptEditDialog::EditType::concept
    );
    const Concept after(d.GetConcept());
    QVERIFY(concept.GetName() == after.GetName());
    QVERIFY(CollectExamplesTexts(concept) == CollectExamplesTexts(after));
  }
}

