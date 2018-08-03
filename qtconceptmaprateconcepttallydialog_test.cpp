#include "qtconceptmaprateconcepttallydialog_test.h"
#include "qtconceptmaprateconcepttallydialog.h"


#include <sstream>
#include <numeric>

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <QKeyEvent>

#include "conceptmapconceptfactory.h"

#include "conceptmapcenternodefactory.h"
#include "conceptmap.h"
#include "conceptmapfactory.h"
#include "conceptmapconcept.h"
#include "conceptmapedge.h"
#include "conceptmapedgefactory.h"
#include "conceptmapnode.h"
#include "conceptmapnodefactory.h"
#include "conceptmapexample.h"
#include "conceptmapedge.h"
#include "conceptmapexamples.h"

#include "qtconceptmaprating.h"

#include "ui_qtconceptmaprateconcepttallydialog.h"


void ribi::cmap::QtConceptMapRateConceptTallyDialogTest::construct_with_empty_conceptmap()
{
  using namespace ribi::cmap;
  const ConceptMap empty_conceptmap;
  try
  {
    QtRateConceptTallyDialog{empty_conceptmap,
      ribi::cmap::CreateDefaultRating()
    };
    QVERIFY(!"Should not get here");
  }
  catch (const std::invalid_argument& e)
  {
    QVERIFY(std::string(e.what())
      == "Cannot create data from empty concept map"
    );
  }
  catch (...)
  {
    QVERIFY(!"Should not get here");
  }
}


void ribi::cmap::QtConceptMapRateConceptTallyDialogTest::construct_with_test_conceptmap()
{
  using namespace ribi::cmap;
  const ConceptMap conceptmap = ConceptMapFactory().Get6();
  QtRateConceptTallyDialog{conceptmap, ribi::cmap::CreateDefaultRating()};
  QVERIFY("Should be no throw");
}

void ribi::cmap::QtConceptMapRateConceptTallyDialogTest::key_presses()
{
  using namespace ribi::cmap;
  const ConceptMap conceptmap = ConceptMapFactory().Get6();
  QtRateConceptTallyDialog d(conceptmap, ribi::cmap::CreateDefaultRating());
  //Translate
  QTest::keyClick(&d, Qt::Key_T, Qt::ControlModifier | Qt::ShiftModifier);
  //Close
  QTest::keyClick(&d, Qt::Key_Escape);
}

void ribi::cmap::QtConceptMapRateConceptTallyDialogTest::measure_ui_from_test_concept_map()
{
  using namespace ribi::cmap;

  const ConceptMap conceptmap{
    ConceptMapFactory().GetRateConceptTallyDialogExample()
  };
  QtRateConceptTallyDialog d{
    conceptmap, ribi::cmap::CreateDefaultRating()
  };
  d.show();
  d.resize(500, 500);
  for (int i=0; i!=1000; ++i) qApp->processEvents();
  QCOMPARE(d.ui->table->columnCount(), 4);
  QCOMPARE(d.ui->table->rowCount(), 5);
  QVERIFY(boost::num_vertices(conceptmap) == 3);
  QVERIFY(boost::num_edges(conceptmap) == 2);

  // [X] [empty] [empty] via 'prerequisite' verbonden met 'order'
  QVERIFY(d.ui->table->item(0, 0)->flags() == (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable));
  QVERIFY(d.ui->table->item(0, 1)->flags() == Qt::ItemIsEnabled); //Empty
  QVERIFY(d.ui->table->item(0, 2)->flags() == Qt::ItemIsEnabled); //Empty
  QVERIFY(d.ui->table->item(0, 3)->flags() == (Qt::ItemIsSelectable | Qt::ItemIsEnabled));
  QVERIFY(d.ui->table->item(0, 3)->text() == "via 'prerequisite' verbonden met 'Order'");

  // [X] [C] [S] Always establish order first
  QVERIFY(d.ui->table->item(1, 0)->flags() == (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable));
  QVERIFY(d.ui->table->item(1, 1)->flags() == (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable));
  QVERIFY(d.ui->table->item(1, 2)->flags() == (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable));
  QVERIFY(d.ui->table->item(1, 3)->flags() == (Qt::ItemIsSelectable | Qt::ItemIsEnabled));
  QVERIFY(d.ui->table->item(1, 3)->text() == "Always establish order first");

  // [X] [C] [S] Punishment
  QVERIFY(d.ui->table->item(2, 0)->flags() == (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable));
  QVERIFY(d.ui->table->item(2, 1)->flags() == (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable));
  QVERIFY(d.ui->table->item(2, 2)->flags() == (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable));
  QVERIFY(d.ui->table->item(2, 3)->flags() == (Qt::ItemIsSelectable | Qt::ItemIsEnabled));
  QVERIFY(d.ui->table->item(2, 3)->text() == "Punishment");

  // [X] [empty] [empty] via 'strengthen' verbonden met 'order'
  QVERIFY(d.ui->table->item(3, 0)->flags() == (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable));
  QVERIFY(d.ui->table->item(3, 1)->flags() == Qt::ItemIsEnabled); //Empty
  QVERIFY(d.ui->table->item(3, 2)->flags() == Qt::ItemIsEnabled); //Empty
  QVERIFY(d.ui->table->item(3, 3)->flags() == (Qt::ItemIsSelectable | Qt::ItemIsEnabled));
  QVERIFY(d.ui->table->item(3, 3)->text() == "via 'strengthen' verbonden met 'Order'");

  // [X] [C] [S] Students teaching each other get to know each other
  QVERIFY(d.ui->table->item(4, 0)->flags() == (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable));
  QVERIFY(d.ui->table->item(4, 1)->flags() == (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable));
  QVERIFY(d.ui->table->item(4, 2)->flags() == (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable));
  QVERIFY(d.ui->table->item(4, 3)->flags() == (Qt::ItemIsSelectable | Qt::ItemIsEnabled));
  QVERIFY(d.ui->table->item(4, 3)->text() == "Students teaching each other, get to know each other");

  #ifdef REALLY_TEST_DEEPER
  d.Write(conceptmap);
  QVERIFY(d == ConceptMapFactory().GetRateConceptTallyDialogExample());

  //Modify first row
  d.ui->table->item(0, 0)->setCheckState(d.ui->table->item(0, 0)->checkState() == Qt::Unchecked ? Qt::Checked : Qt::Unchecked);
  d.ui->table->item(0, 3)->setText("order (as in peace and quiet)");

  //Modify second row
  d.ui->table->item(1, 0)->setCheckState(d.ui->table->item(1, 0)->checkState() == Qt::Unchecked ? Qt::Checked : Qt::Unchecked);
  d.ui->table->item(1, 1)->setCheckState(d.ui->table->item(1, 1)->checkState() == Qt::Unchecked ? Qt::Checked : Qt::Unchecked);
  d.ui->table->item(1, 2)->setCheckState(d.ui->table->item(1, 2)->checkState() == Qt::Unchecked ? Qt::Checked : Qt::Unchecked);
  d.ui->table->item(1, 3)->setText("Order should be established first");
  #endif // REALLY_TEST_DEEPER

}

void ribi::cmap::QtConceptMapRateConceptTallyDialogTest::unchecking_decreases_score()
{
  using namespace ribi::cmap;

  const ConceptMap conceptmap{
    ConceptMapFactory().GetRateConceptTallyDialogExample()
  };
  QtRateConceptTallyDialog d{
    conceptmap, ribi::cmap::CreateDefaultRating()
  };
  d.show();
  d.resize(500, 500);
  assert(d.GetSuggestedComplexity() == 2);
  assert(d.GetSuggestedConcreteness() == 1);
  assert(d.GetSuggestedSpecificity() == 1);

  // [X] [empty] [empty] via 'prerequisite' verbonden met 'order'
  d.ui->table->item(0, 0)->setCheckState(Qt::Unchecked);

  // [X] [C] [S] Always establish order first
  d.ui->table->item(1, 0)->setCheckState(Qt::Unchecked);
  d.ui->table->item(1, 1)->setCheckState(Qt::Unchecked);
  d.ui->table->item(1, 2)->setCheckState(Qt::Unchecked);

  // [X] [C] [S] Punishment
  d.ui->table->item(2, 0)->setCheckState(Qt::Unchecked);
  d.ui->table->item(2, 1)->setCheckState(Qt::Unchecked);
  d.ui->table->item(2, 2)->setCheckState(Qt::Unchecked);

  // [X] [empty] [empty] via 'strengthen' verbonden met 'order'
  d.ui->table->item(3, 0)->setCheckState(Qt::Unchecked);

  // [X] [C] [S] Students teaching each other get to know each other
  d.ui->table->item(4, 0)->setCheckState(Qt::Unchecked);
  d.ui->table->item(4, 1)->setCheckState(Qt::Unchecked);
  d.ui->table->item(4, 2)->setCheckState(Qt::Unchecked);

  QSKIP("NOT NOW, #252", "ISSUE 252");
  assert(d.GetSuggestedComplexity() == 0);
  assert(d.GetSuggestedConcreteness() == 0);
  assert(d.GetSuggestedSpecificity() == 0);
}
