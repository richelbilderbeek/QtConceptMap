#include "qtconceptmapeditconceptdialog_test.h"
#include "qtconceptmapeditconceptdialog.h"

#include <QKeyEvent>

#include "conceptmapcompetency.h"
#include "conceptmapexample.h"
#include "conceptmapexamplefactory.h"
#include "conceptmapexamplesfactory.h"
#include "conceptmapexamples.h"
#include "conceptmaphelper.h"
#include "conceptmapconcept.h"
#include "conceptmapconceptfactory.h"
#include "qtconceptmapcompetency.h"
#include "ui_qtconceptmapeditconceptdialog.h"

using namespace ribi::cmap;
using EditType = ribi::cmap::QtConceptMapConceptEditDialog::EditType;

void ribi::cmap::QtEditDialogTest
  ::ConstructConceptWithOneExample() const noexcept
{
  QtConceptMapConceptEditDialog d(
    Concept("concept", Examples( { Example("example") } ) ),
    EditType::concept
  );
  d.show();
}

void ribi::cmap::QtEditDialogTest
  ::ConstructConceptWithTwoExamples() const noexcept
{
  QtConceptMapConceptEditDialog d(
    Concept("concept", Examples( { Example("example 1"), Example("example 2") } ) ),
    EditType::concept
  );
  d.show();
}


void ribi::cmap::QtEditDialogTest
  ::ConstructConceptWithoutExamples() const noexcept
{
  QtConceptMapConceptEditDialog d(
    Concept("concept"),
    EditType::concept
  );
  d.show();
}

void ribi::cmap::QtEditDialogTest
  ::ConstructRelationWithoutExamples() const noexcept
{
  QtConceptMapConceptEditDialog d(
    Concept("relation"),
    EditType::relation
  );
  d.show();
}

void ribi::cmap::QtEditDialogTest
  ::PressOkWithChangingExamplesResultsInChangedConcept() const noexcept
{
  //Assume reading in a concept and clicking OK after adding an example
  for (const auto concept: ConceptFactory().GetTests())
  {
    QtConceptMapConceptEditDialog d(
      concept,
      EditType::concept
    );
    QVERIFY(d.ui->edit_text->toPlainText().isEmpty());
    d.ui->edit_text->setPlainText("TO BE ADDED EXAMPLE");
    d.on_button_add_clicked(); //Adds text
    const Concept after(d.ToConcept());
    QVERIFY(concept.GetName() == after.GetName());
    QVERIFY(CollectExamplesTexts(concept) != CollectExamplesTexts(after));
  }
}



void ribi::cmap::QtEditDialogTest
  ::PressOkWithChangingNameResultsInChangedConcept() const noexcept
{
  //Assume reading in a concept and clicking OK after modification of the name does modify concept
  for (const auto concept: ConceptFactory().GetTests())
  {
    QtConceptMapConceptEditDialog d(
      concept,
      EditType::concept
    );
    d.ui->edit_concept->setPlainText(d.ui->edit_concept->toPlainText() + "MODIFICATION");
    const Concept after(d.ToConcept());
    QVERIFY(concept.GetName() != after.GetName());
    QVERIFY(CollectExamplesTexts(concept) == CollectExamplesTexts(after));
  }
}


void ribi::cmap::QtEditDialogTest
  ::PressOkWithoutChangesResultsInUnchangedConcept() const noexcept
{
  //Assume reading in a concept and clicking OK without modification does not modify anything
  for (const auto concept: ConceptFactory().GetTests())
  {
    QtConceptMapConceptEditDialog d(
      concept,
      EditType::concept
    );
    const Concept after(d.ToConcept());
    QVERIFY(concept.GetName() == after.GetName());
    QVERIFY(CollectExamplesTexts(concept) == CollectExamplesTexts(after));
  }
}

