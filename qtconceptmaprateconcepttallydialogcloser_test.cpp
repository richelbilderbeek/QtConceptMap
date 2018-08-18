#include "qtconceptmaprateconcepttallydialogcloser_test.h"

#include "qtconceptmaprateconcepttallydialogcloser.h"
#include "qtconceptmaprateconcepttallydialog.h"
#include "conceptmapfactory.h"

using namespace ribi::cmap;

void ribi::cmap::QtConceptMapRateConceptTallyDialogCloserTest::Close()
{
  QtRateConceptTallyDialogCloser c;

  QTimer::singleShot(100, &c, SLOT(Close()));
  ribi::cmap::QtRateConceptTallyDialog d(
    ConceptMapFactory().GetRateConceptTallyDialogExample(),
    CreateDefaultRating()
  );
  d.exec();
  QVERIFY("Closed the dialog (would freeze otherwise)");
}
