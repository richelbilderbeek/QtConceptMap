#include "qtconceptmapexamplesdialog_test.h"

#ifdef REALLY_NEED_THIS_20180818

#include "qtconceptmapexamplesdialog.h"

#include <boost/bind/bind.hpp>
#include <boost/lambda/lambda.hpp>

#include <QLabel>
#include <QLayout>
#include <QVBoxLayout>

#include "conceptmapexample.h"
#include "conceptmapexamples.h"
#include "conceptmapexamplesfactory.h"
#include "qtconceptmapexampledialog.h"



void ribi::cmap::QtConceptMapExamplesDialogTest::all_tests()
{
  using namespace ribi::cmap;
  QtExamplesDialog d;
  for(const auto examples: ExamplesFactory().GetTests())
  {
    d.SetExamples(examples);
    QVERIFY(d.GetExamples() == examples);
  }
}

#endif // REALLY_NEED_THIS_20180818
