#include "qtconceptmaprateconceptdialogcloser_test.h"

#include "conceptmapfactory.h"
#include "conceptmaphelper.h"
#include "qtconceptmap.h"
#include "qtconceptmaprateconceptdialog.h"
#include "qtconceptmaprateconceptdialogcloser.h"

using namespace ribi::cmap;

void ribi::cmap::QtRateConceptDialogCloserTest::Modify() const noexcept
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
