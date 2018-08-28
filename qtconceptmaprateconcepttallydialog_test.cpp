#include "qtconceptmaprateconcepttallydialog_test.h"
#include "qtconceptmaprateconcepttallydialog.h"

#include <sstream>
#include <numeric>

#include <vector>
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

using namespace ribi::cmap;

void ribi::cmap::QtRateConceptTallyDialogTest::ConstructWithEmptyConceptmap() const noexcept
{
  const ConceptMap empty_conceptmap;
  try
  {
    QtRateConceptTallyDialog{
      empty_conceptmap, CreateDefaultRating()
    };
    assert(!"Should not get here");
  }
  catch (const std::invalid_argument& e)
  {
    QVERIFY(std::string(e.what())
      == "Cannot create data from empty concept map"
    );
  }
}


void ribi::cmap::QtRateConceptTallyDialogTest::ConstructWithTestConceptmap() const noexcept
{
  const ConceptMap conceptmap{
    ConceptMapFactory().GetRateConceptTallyDialogExample()
  };
  QtRateConceptTallyDialog{conceptmap, CreateDefaultRating()};
  QVERIFY("Should be no throw");
}

void ribi::cmap::QtRateConceptTallyDialogTest::GivesCorrectSuggestions() const noexcept
{
  // From https://github.com/richelbilderbeek/BrainWeaver/issues/293
  // [...] bij 2 complexe relaties en 0 of 1 complexe voorbeelden
  // [...] geeft de computer complexiteit 2 en dat moet 1 zijn

  const ConceptMap conceptmap{
    ConceptMapFactory().GetRateConceptTallyDialogExample293()
  };
  QtRateConceptTallyDialog d{
    conceptmap, CreateDefaultRating()
  };
  d.show();
  qDebug() << d.GetSuggestedConcreteness() << d.GetSuggestedSpecificity();
  QVERIFY(d.GetSuggestedConcreteness() == 0); //Unsure
  QVERIFY(d.GetSuggestedSpecificity() == 0); //Unsure
  assert(d.GetSuggestedComplexity() == 1);
  QVERIFY(d.GetSuggestedComplexity() == 1);
}

void ribi::cmap::QtRateConceptTallyDialogTest::KeyPresses() const noexcept
{
  const ConceptMap conceptmap = ConceptMapFactory().Get6();
  QtRateConceptTallyDialog d(conceptmap, CreateDefaultRating());
  //Translate
  QTest::keyClick(&d, Qt::Key_T, Qt::ControlModifier | Qt::ShiftModifier);
  //Close
  QTest::keyClick(&d, Qt::Key_Escape);
  QVERIFY(d.isHidden());
}

void ribi::cmap::QtRateConceptTallyDialogTest::MeasureUiFromTestConceptmap() const noexcept
{
  const ConceptMap conceptmap{
    ConceptMapFactory().GetRateConceptTallyDialogExample()
  };
  QtRateConceptTallyDialog d{
    conceptmap, CreateDefaultRating()
  };
  d.show();
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
}

void ribi::cmap::QtRateConceptTallyDialogTest::UncheckingDecreasesSuggestion() const noexcept
{

  const ConceptMap conceptmap{
    ConceptMapFactory().GetRateConceptTallyDialogExample()
  };
  QtRateConceptTallyDialog d{
    conceptmap, CreateDefaultRating()
  };
  d.show();
  assert(d.GetSuggestedComplexity() == 2);
  assert(d.GetSuggestedConcreteness() == 1);
  assert(d.GetSuggestedSpecificity() == 1);

  //Uncheck all
  for (int col = 0; col != 3; ++col)
  {
    for (int row = 0; row != 5; ++row)
    {
      QTableWidgetItem * const item = d.ui->table->item(row, col);
      if (item && (item->flags() & Qt::ItemIsUserCheckable))
      {
        item->setCheckState(Qt::Unchecked);
      }
    }
  }

  QVERIFY(d.GetSuggestedComplexity() == 0);
  QVERIFY(d.GetSuggestedConcreteness() == 0);
  QVERIFY(d.GetSuggestedSpecificity() == 0);
}

void ribi::cmap::QtRateConceptTallyDialogTest::UserHasNotClickedOkAtConstruction() const noexcept
{
  const ConceptMap conceptmap{
    ConceptMapFactory().GetRateConceptTallyDialogExample()
  };
  QtRateConceptTallyDialog d{conceptmap, CreateDefaultRating()};
  QVERIFY(!d.HasUserClickedOk());
}

void ribi::cmap::QtRateConceptTallyDialogTest::Write() const noexcept
{
  const ConceptMap conceptmap{
    ConceptMapFactory().GetRateConceptTallyDialogExample()
  };
  QtRateConceptTallyDialog d{conceptmap, CreateDefaultRating()};
  ConceptMap conceptmap_out = conceptmap;
  d.Write(conceptmap_out);
  assert(HasSimilarData(conceptmap, conceptmap_out, 0.0001));
  assert(d.ui->table->item(0, 0)->checkState() == Qt::Checked);
  d.ui->table->item(0, 0)->setCheckState(Qt::Unchecked);
  d.Write(conceptmap_out);
  QVERIFY(!HasSimilarData(conceptmap, conceptmap_out, 0.0001));
}
