#include "qtconceptmaprateconceptdialog_test.h"

#include "qtconceptmap.h"
#include "qtconceptmaprateconceptdialog.h"
#include "conceptmapfactory.h"



void ribi::cmap::QtConceptMapRateConceptDialogTest::ShowNormalConceptMap()
{
  auto qtconceptmap{
    std::make_unique<QtConceptMap>()
  };
  qtconceptmap->SetConceptMap(ConceptMapFactory().GetUnrated());
  const auto * const qtnode = GetFirstQtNode(*qtconceptmap);
  assert(qtnode);
  auto d{
    std::make_unique<QtRateConceptDialog>(
      *qtconceptmap, *qtnode
    )
  };
  d->show();
}

void ribi::cmap::QtConceptMapRateConceptDialogTest::ShowConceptMapWithOnlyCenterNode()
{
  auto qtconceptmap{
    std::make_unique<QtConceptMap>()
  };
  qtconceptmap->SetConceptMap(ConceptMapFactory().Get1());
  const auto * const qtnode = GetFirstQtNode(*qtconceptmap);
  assert(qtnode);
  auto d{
    std::make_unique<QtRateConceptDialog>(
      *qtconceptmap, *qtnode
    )
  };
  d->show();
}

void ribi::cmap::QtConceptMapRateConceptDialogTest::UserHasNotPressedOkAtConstruction()
{
  auto qtconceptmap{
    std::make_unique<QtConceptMap>()
  };
  qtconceptmap->SetConceptMap(ConceptMapFactory().Get1());
  const auto * const qtnode = GetFirstQtNode(*qtconceptmap);
  assert(qtnode);
  auto d{
    std::make_unique<QtRateConceptDialog>(
      *qtconceptmap, *qtnode
    )
  };
  QVERIFY(!d->GetOkClicked());
}

void ribi::cmap::QtConceptMapRateConceptDialogTest::XcsAreReadFromQtConceptMap()
{
  ConceptMap conceptmap;
  const bool is_complex{true};
  const int complexity{0};
  const int concreteness{1};
  const int specificity{2};
  AddVertex(
    Node(
      Concept(
        "XCS", Examples(), is_complex, complexity, concreteness, specificity
      )
    ),
    conceptmap
  );
  auto qtconceptmap{
    std::make_unique<QtConceptMap>()
  };
  qtconceptmap->SetConceptMap(conceptmap);
  const auto * const qtnode = GetFirstQtNode(*qtconceptmap);
  assert(qtnode);
  auto d{
    std::make_unique<QtRateConceptDialog>(
      *qtconceptmap, *qtnode
    )
  };
  QVERIFY(d->GetComplexity() == complexity);
  QVERIFY(d->GetConcreteness() == concreteness);
  QVERIFY(d->GetSpecificity() == specificity);
}
