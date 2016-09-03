#include "qtconceptmaprateexamplesdialog_test.h"
#include "qtconceptmaprateexamplesdialog.h"


#include <QDesktopWidget>
#include <QKeyEvent>
#include <QListWidgetItem>

#include "conceptmapconceptfactory.h"
#include "conceptmapconcept.h"
#include "conceptmapexamplefactory.h"
#include "conceptmapexamplesfactory.h"
#include "conceptmaphelper.h"
#include "qtconceptmapcompetency.h"

#include "trace.h"
#include "ui_qtconceptmaprateexamplesdialog.h"

void ribi::cmap::qtconceptmaprateexamplesdialog_test::all_tests()
{
  using namespace ribi::cmap;
  {
    const int sz = ConceptFactory().GetTests().size();
    for (int i=0; i!=sz; ++i)
    {
      QVERIFY(i < static_cast<int>(ConceptFactory().GetTests().size()));
      const auto a = QtRateExamplesDialogNewName(ConceptFactory().GetTests()[i]).GetRatedExamples();
      for (int j=0; j!=sz; ++j)
      {
        QVERIFY(j < static_cast<int>(ConceptFactory().GetTests().size()));
        const auto b = QtRateExamplesDialogNewName(ConceptFactory().GetTests()[j]).GetRatedExamples();
        if (i == j)
        {
          QVERIFY(a == b);
        }
        else
        {
          QVERIFY(a != b);
        }
      }
    }
  }
}
