#include "qtconceptmaprateconcepttallydialogcloser_test.h"

#include "conceptmapfactory.h"
#include "conceptmaphelper.h"
#include "qtconceptmaprateconcepttallydialog.h"
#include "qtconceptmaprateconcepttallydialogcloser.h"

using namespace ribi::cmap;

void ribi::cmap::QtRateConceptTallyDialogCloserTest::PressCancel() const noexcept
{
  QtRateConceptTallyDialogCloser c;

  if (OnTravis()) return;

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
  QtRateConceptTallyDialogCloser c;

  if (OnTravis()) return;

  QTimer::singleShot(100, &c, SLOT(PressOk()));
  ribi::cmap::QtRateConceptTallyDialog d(
    ConceptMapFactory().GetRateConceptTallyDialogExample(),
    CreateDefaultRating()
  );
  d.exec();
  QVERIFY(d.HasUserClickedOk());
}
