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

void ribi::cmap::QtEditConceptDialogTest::ClickCancelIsNoted() const noexcept
{
  const Concept concept;
  QtEditConceptDialog d(concept, QtEditConceptDialog::EditType::concept);
  d.on_button_cancel_clicked();
  QVERIFY(!d.HasUserClickedOk());

}

void ribi::cmap::QtEditConceptDialogTest::ClickOkIsNoted() const noexcept
{
  const Concept concept;
  QtEditConceptDialog d(concept, QtEditConceptDialog::EditType::concept);
  d.on_button_ok_clicked();
  QVERIFY(d.HasUserClickedOk());
}

void ribi::cmap::QtEditConceptDialogTest::ConstructConceptWithOneExample() const noexcept
{
  QtEditConceptDialog d(
    Concept("concept", Examples( { Example("example") } ) ),
    QtEditConceptDialog::concept
  );
  d.show();
}

void ribi::cmap::QtEditConceptDialogTest
  ::ConstructConceptWithTwoExamples() const noexcept
{
  QtEditConceptDialog d(
    Concept("concept", Examples( { Example("example 1"), Example("example 2") } ) ),
    QtEditConceptDialog::concept
  );
  d.show();
}


void ribi::cmap::QtEditConceptDialogTest
  ::ConstructConceptWithoutExamples() const noexcept
{
  QtEditConceptDialog d(
    Concept("concept"),
    QtEditConceptDialog::concept
  );
  d.show();
}

void ribi::cmap::QtEditConceptDialogTest
  ::ConstructRelationWithoutExamples() const noexcept
{
  QtEditConceptDialog d(
    Concept("relation"),
    QtEditConceptDialog::relation
  );
  d.show();
}

void ribi::cmap::QtEditConceptDialogTest
  ::PressOkWithChangingExamplesResultsInChangedConcept() const noexcept
{
  //Assume reading in a concept and clicking OK after adding an example
  for (const auto concept: ConceptFactory().GetTests())
  {
    QtEditConceptDialog d(
      concept,
      QtEditConceptDialog::concept
    );
    QVERIFY(d.ui->edit_text->toPlainText().isEmpty());
    d.ui->edit_text->setPlainText("TO BE ADDED EXAMPLE");
    d.on_button_add_clicked(); //Adds text
    const Concept after(d.ToConcept());
    QVERIFY(concept.GetName() == after.GetName());
    QVERIFY(CollectExamplesTexts(concept) != CollectExamplesTexts(after));
  }
}



void ribi::cmap::QtEditConceptDialogTest
  ::PressOkWithChangingNameResultsInChangedConcept() const noexcept
{
  //Assume reading in a concept and clicking OK after modification of the name does modify concept
  for (const auto concept: ConceptFactory().GetTests())
  {
    QtEditConceptDialog d(
      concept,
      QtEditConceptDialog::concept
    );
    d.ui->edit_concept->setPlainText(d.ui->edit_concept->toPlainText() + "MODIFICATION");
    const Concept after(d.ToConcept());
    QVERIFY(concept.GetName() != after.GetName());
    QVERIFY(CollectExamplesTexts(concept) == CollectExamplesTexts(after));
  }
}


void ribi::cmap::QtEditConceptDialogTest
  ::PressOkWithoutChangesResultsInUnchangedConcept() const noexcept
{
  //Assume reading in a concept and clicking OK without modification does not modify anything
  for (const auto concept: ConceptFactory().GetTests())
  {
    QtEditConceptDialog d(
      concept,
      QtEditConceptDialog::concept
    );
    const Concept after(d.ToConcept());
    QVERIFY(concept.GetName() == after.GetName());
    QVERIFY(CollectExamplesTexts(concept) == CollectExamplesTexts(after));
  }
}


void ribi::cmap::QtEditConceptDialogTest
  ::UserHasNotClickedOkAtConstruction() const noexcept
{
  const QtEditConceptDialog d(Concept(), QtEditConceptDialog::concept);
  QVERIFY(!d.HasUserClickedOk());
}
