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
#pragma GCC diagnostic pop

void ribi::cmap::qtconceptmaprateconcepttallydialog_test::construct_with_empty_conceptmap()
{
  using namespace ribi::cmap;
  const ConceptMap empty_conceptmap;
  try
  {
    QtRateConceptTallyDialog{empty_conceptmap};
    QVERIFY(!"Should not get here");
  }
  catch (std::logic_error& e)
  {
    QVERIFY("Should get here");
  }
  catch (...)
  {
    QVERIFY(!"Should not get here");
  }
}


void ribi::cmap::qtconceptmaprateconcepttallydialog_test::construct_with_test_conceptmap()
{
  using namespace ribi::cmap;
  const ConceptMap conceptmap = ConceptMapFactory().Get6();
  QtRateConceptTallyDialog{conceptmap};
  QVERIFY("Should be no throw");
}

void ribi::cmap::qtconceptmaprateconcepttallydialog_test::measure_ui_from_test_concept_map()
{
  return; // TODO, #define NOT_NOW_20160327

  using namespace ribi::cmap;

  const ConceptMap conceptmap = ConceptMapFactory().Get6();
  QtRateConceptTallyDialog d{conceptmap};
  d.show();
  for (int i=0; i!=1000; ++i) qApp->processEvents();

  QCOMPARE(d.GetUi()->table->columnCount(), 4);
  QCOMPARE(d.GetUi()->table->rowCount(), 3);
  QCOMPARE(boost::num_vertices(conceptmap), 2);
  QCOMPARE(boost::num_edges(conceptmap), 1);
  const Node focal_node = GetFirstNode(conceptmap);
  //const Node other_node = conceptmap.GetNodes()[1]; //Don't care
  const Edge edge = ribi::cmap::GetFirstEdge(conceptmap);

  QVERIFY(d.GetUi()->table->item(0,0)->flags() == (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable));
  QVERIFY(d.GetUi()->table->item(0,1)->flags() == (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable));
  QVERIFY(d.GetUi()->table->item(0,2)->flags() == (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable));
  QVERIFY(d.GetUi()->table->item(0,3)->flags() == (Qt::ItemIsSelectable | Qt::ItemIsEnabled));

  QVERIFY(d.GetUi()->table->item(1,0)->flags() == (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable));
  QVERIFY(d.GetUi()->table->item(1,1)->flags() == Qt::ItemIsEnabled); //Empty
  QVERIFY(d.GetUi()->table->item(1,2)->flags() == Qt::ItemIsEnabled); //Empty
  QVERIFY(d.GetUi()->table->item(1,3)->flags() == (Qt::ItemIsSelectable | Qt::ItemIsEnabled));

  QVERIFY(d.GetUi()->table->item(2,0)->flags() == (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable));
  QVERIFY(d.GetUi()->table->item(2,1)->flags() == (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable));
  QVERIFY(d.GetUi()->table->item(2,2)->flags() == (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable));
  QVERIFY(d.GetUi()->table->item(2,3)->flags() == (Qt::ItemIsSelectable | Qt::ItemIsEnabled));

  //Check current state, before modification

  QVERIFY(d.GetUi()->table->item(0,0)->checkState() == (focal_node.GetConcept().GetExamples().Get()[0].GetIsComplex() ? Qt::Checked : Qt::Unchecked));
  QVERIFY(d.GetUi()->table->item(0,1)->checkState() == (focal_node.GetConcept().GetExamples().Get()[0].GetIsConcrete() ? Qt::Checked : Qt::Unchecked));
  QVERIFY(d.GetUi()->table->item(0,2)->checkState() == (focal_node.GetConcept().GetExamples().Get()[0].GetIsSpecific() ? Qt::Checked : Qt::Unchecked));
  QVERIFY(d.GetUi()->table->item(0,3)->text() == QString(focal_node.GetConcept().GetExamples().Get()[0].GetText().c_str()));

  QVERIFY(d.GetUi()->table->item(1,0)->checkState() == (edge.GetNode().GetConcept().GetIsComplex() ? Qt::Checked : Qt::Unchecked));
  QVERIFY(d.GetUi()->table->item(1,1)->text() == "");
  QVERIFY(d.GetUi()->table->item(1,2)->text() == "");
  //NEW 20131231: now the text contains both
  //- the concept name of the edge
  //- the name of the node the edge is connected to
  #ifdef NOT_NOW_20160201
  QVERIFY(d.GetUi()->table->item(1,3)->text().toStdString().find(edge.GetNode().GetConcept().GetName()) != std::string::npos);
  QVERIFY(d.GetUi()->table->item(1,3)->text().toStdString().find(edge.GetTo()->GetConcept().GetName()) != std::string::npos);
  //OLD QVERIFY(d.GetUi()->table->item(1,3)->text() == QString(edge.GetConcept().GetName().c_str()));
  #endif // NOT_NOW_20160201

  QVERIFY(d.GetUi()->table->item(2,0)->checkState() == (edge.GetNode().GetConcept().GetExamples().Get()[0].GetIsComplex() ? Qt::Checked : Qt::Unchecked));
  QVERIFY(d.GetUi()->table->item(2,1)->checkState() == (edge.GetNode().GetConcept().GetExamples().Get()[0].GetIsConcrete() ? Qt::Checked : Qt::Unchecked));
  QVERIFY(d.GetUi()->table->item(2,2)->checkState() == (edge.GetNode().GetConcept().GetExamples().Get()[0].GetIsSpecific() ? Qt::Checked : Qt::Unchecked));
  QVERIFY(d.GetUi()->table->item(2,3)->text() == QString(edge.GetNode().GetConcept().GetExamples().Get()[0].GetText().c_str()));

  //Modify table
  d.GetUi()->table->item(0,0)->setCheckState(d.GetUi()->table->item(0,0)->checkState() == Qt::Unchecked ? Qt::Checked : Qt::Unchecked);
  d.GetUi()->table->item(0,1)->setCheckState(d.GetUi()->table->item(0,1)->checkState() == Qt::Unchecked ? Qt::Checked : Qt::Unchecked);
  d.GetUi()->table->item(0,2)->setCheckState(d.GetUi()->table->item(0,2)->checkState() == Qt::Unchecked ? Qt::Checked : Qt::Unchecked);
  //d.GetUi()->table->item(0,3)->setText("MODIFIED"); //User should not be able to modify this

  d.GetUi()->table->item(1,0)->setCheckState(d.GetUi()->table->item(1,0)->checkState() == Qt::Unchecked ? Qt::Checked : Qt::Unchecked);
  //d.GetUi()->table->item(1,3)->setText("MODIFIED TOO"); //User should not be able to modify this

  d.GetUi()->table->item(2,0)->setCheckState(d.GetUi()->table->item(2,0)->checkState() == Qt::Unchecked ? Qt::Checked : Qt::Unchecked);
  d.GetUi()->table->item(2,1)->setCheckState(d.GetUi()->table->item(2,1)->checkState() == Qt::Unchecked ? Qt::Checked : Qt::Unchecked);
  d.GetUi()->table->item(2,2)->setCheckState(d.GetUi()->table->item(2,2)->checkState() == Qt::Unchecked ? Qt::Checked : Qt::Unchecked);
  //d.GetUi()->table->item(2,3)->setText("MODIFIED AS WELL"); //User should not be able to modify this

  //Check that data is modified by GUI

  QVERIFY(d.GetUi()->table->item(0,0)->checkState() == (focal_node.GetConcept().GetExamples().Get()[0].GetIsComplex() ? Qt::Checked : Qt::Unchecked));
  QVERIFY(d.GetUi()->table->item(0,1)->checkState() == (focal_node.GetConcept().GetExamples().Get()[0].GetIsConcrete() ? Qt::Checked : Qt::Unchecked));
  QVERIFY(d.GetUi()->table->item(0,2)->checkState() == (focal_node.GetConcept().GetExamples().Get()[0].GetIsSpecific() ? Qt::Checked : Qt::Unchecked));
  QVERIFY(d.GetUi()->table->item(0,3)->text() == QString(focal_node.GetConcept().GetExamples().Get()[0].GetText().c_str()));

  QVERIFY(d.GetUi()->table->item(1,0)->checkState() == (edge.GetNode().GetConcept().GetIsComplex() ? Qt::Checked : Qt::Unchecked));
  QVERIFY(d.GetUi()->table->item(1,1)->text() == "");
  QVERIFY(d.GetUi()->table->item(1,2)->text() == "");

  //NEW 20131231: now the text contains both
  //- the concept name of the edge
  //- the name of the node the edge is connected to
  #ifdef NOT_NOW_20160201
  QVERIFY(d.GetUi()->table->item(1,3)->text().toStdString().find(edge.GetNode().GetConcept().GetName()) != std::string::npos);
  QVERIFY(d.GetUi()->table->item(1,3)->text().toStdString().find(edge.GetTo()->GetConcept().GetName()) != std::string::npos);
  //OLD QVERIFY(d.GetUi()->table->item(1,3)->text() == QString(edge.GetConcept().GetName().c_str()));
  #endif // NOT_NOW_20160201

  QVERIFY(d.GetUi()->table->item(2,0)->checkState() == (edge.GetNode().GetConcept().GetExamples().Get()[0].GetIsComplex() ? Qt::Checked : Qt::Unchecked));
  QVERIFY(d.GetUi()->table->item(2,1)->checkState() == (edge.GetNode().GetConcept().GetExamples().Get()[0].GetIsConcrete() ? Qt::Checked : Qt::Unchecked));
  QVERIFY(d.GetUi()->table->item(2,2)->checkState() == (edge.GetNode().GetConcept().GetExamples().Get()[0].GetIsSpecific() ? Qt::Checked : Qt::Unchecked));
  QVERIFY(d.GetUi()->table->item(2,3)->text() == QString(edge.GetNode().GetConcept().GetExamples().Get()[0].GetText().c_str()));
}
