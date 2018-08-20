#include "qtconceptmaprateconceptdialog_test.h"

#include "conceptmapfactory.h"
#include "qtconceptmap.h"
#include "qtconceptmaprateconceptdialog.h"
#include "ui_qtconceptmaprateconceptdialog.h"
#include "qtconceptmaprateconcepttallydialog.h"
#include "ui_qtconceptmaprateconcepttallydialog.h"
#include "qtconceptmaprateconcepttallydialogcloser.h"
#include "conceptmaphelper.h"

void ribi::cmap::QtRateConceptDialogTest::EscapeClosesDialog() const
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
  QTest::keyClick(d.get(), Qt::Key_Escape);
  QVERIFY(d->isHidden());
}


void ribi::cmap::QtRateConceptDialogTest::OkClosesDialog() const
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
  assert(!d->isHidden());
  d->ui->button_ok->click();
  QVERIFY(d->isHidden());
}

void ribi::cmap::QtRateConceptDialogTest::OkStoresOkayness() const
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
  QVERIFY(!d->GetOkClicked());
  d->ui->button_ok->click();
  QVERIFY(d->GetOkClicked());
}

void ribi::cmap::QtRateConceptDialogTest::ShowNormalConceptMap() const
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

void ribi::cmap::QtRateConceptDialogTest::ShowConceptMapWithOnlyCenterNode() const
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

void ribi::cmap::QtRateConceptDialogTest::TallyRelevanciesCloses() const
{
  if (OnTravis()) return;
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
  QTest::qWaitForWindowActive(d.get());
  assert(qApp->activeWindow() == d.get());
  QtRateConceptTallyDialogCloser p;
  QTimer::singleShot(
    100,
    &p,
    SLOT(Close())
  );
  d->ui->button_tally_relevancies->click(); //Closed by QTimer
}

void ribi::cmap::QtRateConceptDialogTest::TallyRelevanciesPopsUp() const
{
  if (OnTravis()) return;
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
  QTest::qWaitForWindowActive(d.get());
  assert(qApp->activeWindow() == d.get());
  QTimer::singleShot(100, qApp, SLOT(closeAllWindows()));
  d->ui->button_tally_relevancies->click();
  QVERIFY(qApp->activeWindow() != d.get());
}

void ribi::cmap::QtRateConceptDialogTest::UserHasNotPressedOkAtConstruction() const
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

void ribi::cmap::QtRateConceptDialogTest::XcsAreReadFromQtConceptMap() const
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

void ribi::cmap::QtRateConceptDialogTest::Write() const
{
  auto qtconceptmap{
    std::make_unique<QtConceptMap>()
  };
  qtconceptmap->SetConceptMap(ConceptMapFactory().Get1());
  auto * const qtnode = GetFirstQtNode(*qtconceptmap);
  assert(qtnode);
  auto d{
    std::make_unique<QtRateConceptDialog>(
      *qtconceptmap, *qtnode
    )
  };
  d->show();
  d->Write(*qtconceptmap.get(), *qtnode);
}
