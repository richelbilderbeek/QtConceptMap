#include "qtconceptmaprateconceptdialogcloser_test.h"

#include "conceptmapfactory.h"
#include "conceptmaphelper.h"
#include "qtconceptmap.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmaprateconceptdialog.h"
#include "qtconceptmaprateconceptdialogcloser.h"

using namespace ribi::cmap;

void ribi::cmap::QtRateConceptDialogCloserTest::Modify() const noexcept
{
  if (OnTravis()) return;

  QtRateConceptDialogCloser c;
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetUnrated());
  const QtNode& qtnode = *GetFirstQtNode(q);
  QTimer::singleShot(100, &c, SLOT(Modify()));
  QTimer::singleShot(200, &c, SLOT(PressOk()));
  QtRateConceptDialog d(q, qtnode);
  d.exec();
  assert(d.HasUserClickedOk());
  QVERIFY(GetRatingComplexity(qtnode) != d.GetComplexity());
  QVERIFY(GetRatingConcreteness(qtnode) != d.GetConcreteness());
  QVERIFY(GetRatingSpecificity(qtnode) != d.GetSpecificity());
}

void ribi::cmap::QtRateConceptDialogCloserTest::PressCancel() const noexcept
{
  if (OnTravis()) return;

  QtRateConceptDialogCloser c;


  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetUnrated());

  QTimer::singleShot(100, &c, SLOT(PressCancel()));
  QtRateConceptDialog d(q, *GetFirstQtNode(q));
  d.exec();
  QVERIFY(!d.HasUserClickedOk());
}

void ribi::cmap::QtRateConceptDialogCloserTest::PressOk() const noexcept
{
  if (OnTravis()) return;

  QtRateConceptDialogCloser c;


  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetUnrated());

  QTimer::singleShot(100, &c, SLOT(PressOk()));
  QtRateConceptDialog d(q, *GetFirstQtNode(q));
  d.exec();
  QVERIFY(d.HasUserClickedOk());
}
