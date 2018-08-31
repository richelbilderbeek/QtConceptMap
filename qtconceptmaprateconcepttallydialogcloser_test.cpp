#include "qtconceptmaprateconcepttallydialogcloser_test.h"

#include "conceptmapfactory.h"
#include "conceptmaphelper.h"
#include "qtconceptmaprateconcepttallydialog.h"
#include "qtconceptmaprateconcepttallydialogcloser.h"

using namespace ribi::cmap;

void ribi::cmap::QtRateConceptTallyDialogCloserTest::Modify() const noexcept
{
  if (OnTravis()) return;

  QtRateConceptTallyDialogCloser c;
  const ConceptMap conceptmap_before = ConceptMapFactory().GetRateConceptTallyDialogExample();
  ConceptMap conceptmap_after = conceptmap_before;
  ribi::cmap::QtRateConceptTallyDialog d(
    conceptmap_after,
    CreateDefaultRating()
  );
  QTimer::singleShot(100, &c, SLOT(Modify()));
  QTimer::singleShot(200, &c, SLOT(PressOk()));
  assert(conceptmap_before == conceptmap_after);
  d.exec();
  d.Write(conceptmap_after);

  QVERIFY(conceptmap_before != conceptmap_after);
}

void ribi::cmap::QtRateConceptTallyDialogCloserTest::PressCancel() const noexcept
{
  if (OnTravis()) return;

  QtRateConceptTallyDialogCloser c;

  QTimer::singleShot(100, &c, SLOT(PressCancel()));
  ribi::cmap::QtRateConceptTallyDialog d(
    ConceptMapFactory().GetRateConceptTallyDialogExample(),
    CreateDefaultRating()
  );
  d.exec();
  QVERIFY(!d.HasUserClickedOk());
}

void ribi::cmap::QtRateConceptTallyDialogCloserTest::PressOk() const noexcept
{
  if (OnTravis()) return;

  QtRateConceptTallyDialogCloser c;

  QTimer::singleShot(100, &c, SLOT(PressOk()));
  ribi::cmap::QtRateConceptTallyDialog d(
    ConceptMapFactory().GetRateConceptTallyDialogExample(),
    CreateDefaultRating()
  );
  d.exec();
  QVERIFY(d.HasUserClickedOk());
}
