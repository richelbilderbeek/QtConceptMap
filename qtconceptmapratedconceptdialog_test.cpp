#include "qtconceptmapratedconceptdialog_test.h"

#include "conceptmapfactory.h"
#include "qtconceptmapratedconceptdialog.h"
#include "ui_qtconceptmapratedconceptdialog.h"

void ribi::cmap::QtRatedConceptDialogTest::AssessorHasRatings() const noexcept
{
  const auto conceptmap = ConceptMapFactory().GetQtRatedConceptDialogExample();
  const auto node = GetFirstNode(conceptmap);
  QtConceptMapRatedConceptDialog d(conceptmap, node, Role::assessor);
  QVERIFY(QRegExp("X:., S:., C:.").indexIn(d.ui->label_concept_examples->text()) >= 0);
  QVERIFY(QRegExp(".., X:., S:., C:.").indexIn(d.ui->label_concept_examples->text()) >= 0);
  QVERIFY(QRegExp(".. X:., S:., C:.").indexIn(d.ui->label_cluster_relations->text()) >= 0);
}

void ribi::cmap::QtRatedConceptDialogTest::LonelyNodeHasNoRelations() const noexcept
{
  const auto conceptmap = ConceptMapFactory().GetLonelyQtCenterNode();
  const auto node = GetFirstNode(conceptmap);
  QtConceptMapRatedConceptDialog d(conceptmap, node, Role::student);
  QVERIFY(QRegExp("geen").indexIn(d.ui->label_cluster_relations->text()) >= 0);
}

void ribi::cmap::QtRatedConceptDialogTest::NodeWithRelationsIndicatesThis() const noexcept
{
  const auto conceptmap = ConceptMapFactory().GetQtRatedConceptDialogExample();
  const auto node = GetFirstNode(conceptmap);
  QtConceptMapRatedConceptDialog d(conceptmap, node, Role::student);
  QVERIFY(QRegExp("geen").indexIn(d.ui->label_cluster_relations->text()) == -1);
}

void ribi::cmap::QtRatedConceptDialogTest::StudentHasNoRatings() const noexcept
{
  const auto conceptmap = ConceptMapFactory().GetQtRatedConceptDialogExample();
  const auto node = GetFirstNode(conceptmap);
  QtConceptMapRatedConceptDialog d(conceptmap, node, Role::student);
  QVERIFY(QRegExp("X:., S:., C:.").indexIn(d.ui->label_concept_examples->text()) == -1);
  QVERIFY(QRegExp(".., X:., S:., C:.").indexIn(d.ui->label_concept_examples->text()) == -1);
  QVERIFY(QRegExp(".. X:., S:., C:.").indexIn(d.ui->label_cluster_relations->text()) == -1);
}

