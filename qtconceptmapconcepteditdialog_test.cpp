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

#include "trace.h"

void ribi::cmap::qtconceptmapconcepteditdialog_test::all_tests()
{
  using namespace ribi::cmap;
  {
    //Assume reading in a concept and clicking OK without modification does not modify anything
    const auto v = ribi::cmap::ConceptFactory().GetTests();
    std::for_each(v.begin(),v.end(),
      [](const Concept& concept)
      {
        const Concept old_concept(concept);
        QVERIFY(concept == old_concept);
        QtConceptMapConceptEditDialog d(concept);
        //Do nothing...
        d.on_button_ok_clicked();
        #ifdef CONCEPTMAP_WRITE_TO_CONCEPT
        QVERIFY(d.WriteToConcept() == old_concept);
        #else
        QVERIFY(concept == old_concept);
        #endif
      }
    );
  }
  {
    //Assume reading in a concept and clicking OK after modification of the name does modify concept
    const auto v = ribi::cmap::ConceptFactory().GetTests();
    std::for_each(v.begin(),v.end(),
      [](const Concept& concept)
      {
        const Concept old_concept(concept);
        QVERIFY(concept == old_concept);
        QtConceptMapConceptEditDialog d(concept);
        d.GetUi()->edit_concept->setText(d.GetUi()->edit_concept->text() + "MODIFICATION");
        d.on_button_ok_clicked();
        QVERIFY(d.GetConcept() != old_concept);
      }
    );
  }
  {
    //Assume reading in a concept and clicking OK after adding an example
    const auto v = ribi::cmap::ConceptFactory().GetTests();
    std::for_each(v.begin(),v.end(),
      [](const Concept& concept)
      {
        const Concept old_concept(concept);
        QVERIFY(concept == old_concept);
        QtConceptMapConceptEditDialog d(concept);
        QVERIFY(d.GetUi()->edit_text->text().isEmpty());
        d.GetUi()->edit_text->setText("TO BE ADDED EXAMPLE");
        d.on_button_add_clicked(); //Should add
        d.on_button_ok_clicked();
        QVERIFY(d.GetConcept() != old_concept);
      }
    );
  }
  {
    //Assume reading in a concept and NOT clicking OK does not change the concept,
    //even when having changed the name and examples in the GUI
    const auto v = ribi::cmap::ConceptFactory().GetTests();
    std::for_each(v.begin(),v.end(),
      [](const Concept& concept)
      {
        const Concept old_concept(concept);
        QVERIFY(concept == old_concept);
        QtConceptMapConceptEditDialog d(concept);
        //Change name
        d.GetUi()->edit_concept->setText(d.GetUi()->edit_concept->text() + "MODIFICATION");
        //Change examples
        QVERIFY(d.GetUi()->edit_text->text().isEmpty());
        d.GetUi()->edit_text->setText("TO BE ADDED EXAMPLE");
        d.on_button_add_clicked(); //Should add
        //DO NOT PRESS OK d.on_button_ok_clicked();
        QVERIFY(d.GetConcept() == old_concept);
      }
    );
  }
}
