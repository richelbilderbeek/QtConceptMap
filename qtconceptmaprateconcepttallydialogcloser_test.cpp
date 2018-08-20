#include "qtconceptmaprateconcepttallydialogcloser_test.h"

#include "conceptmapfactory.h"
#include "conceptmaphelper.h"
#include "qtconceptmaprateconcepttallydialog.h"
#include "qtconceptmaprateconcepttallydialogcloser.h"

using namespace ribi::cmap;

void ribi::cmap::QtRateConceptTallyDialogCloserTest::Close()
{
  QtRateConceptTallyDialogCloser c;

  if (OnTravis()) return;

  QTimer::singleShot(100, &c, SLOT(Close()));
  ribi::cmap::QtRateConceptTallyDialog d(
    ConceptMapFactory().GetRateConceptTallyDialogExample(),
    CreateDefaultRating()
  );
  d.exec();
  QVERIFY("Closed the dialog (would freeze otherwise)");
}
