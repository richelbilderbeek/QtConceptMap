



#include "qtconceptmaprateconceptdialog.h"

#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QKeyEvent>

#include "conceptmapconceptfactory.h"
#include "conceptmapconcept.h"
#include "conceptmapfactory.h"
#include "conceptmap.h"
#include "conceptmapedge.h"
#include "conceptmapexamplesfactory.h"
#include "conceptmapexamples.h"
#include "conceptmapnodefactory.h"
#include "conceptmapnode.h"
#include "qtconceptmaprating.h"
#include "qtconceptmap.h"
#include "qtconceptmaprateconcepttallydialog.h"
#include "ui_qtconceptmaprateconceptdialog.h"

ribi::cmap::QtRateConceptDialog::QtRateConceptDialog(
  const ConceptMap conceptmap,
  const Rating& rating,
  QWidget* parent)
  : QDialog(parent),
    ui(new Ui::QtRateConceptDialog),
    m_button_ok_clicked(false),
    m_conceptmap(conceptmap),
    m_widget(new QtConceptMap(rating))
{
  if (!boost::num_vertices(conceptmap))
  {
    throw std::invalid_argument("Need at least one concept");
  }

  ui->setupUi(this);
  m_widget->SetConceptMap(conceptmap);

  assert(m_widget);
  assert(ui->conceptmap_layout);

  ui->conceptmap_layout->addWidget(m_widget.get());
  ui->box_complexity->setCurrentIndex(
    GetFirstNode(conceptmap).GetConcept().GetRatingComplexity()
  );
  ui->box_concreteness->setCurrentIndex(
    GetFirstNode(conceptmap).GetConcept().GetRatingConcreteness()
  );
  ui->box_specificity->setCurrentIndex(
    GetFirstNode(conceptmap).GetConcept().GetRatingSpecificity()
  );
  ui->box_complexity->setFocus();

  //Set suggestions
  DisplaySuggestions();

  //Center the dialog
  {
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->setGeometry(screen.adjusted(64,64,-64,-64));
    this->move( screen.center() - this->rect().center() );
  }
  //The rating by the Tally dialog must be visible as of 2013-08-30
  //so let this dialog follow the ratings done by the tally dialog
}

ribi::cmap::QtRateConceptDialog::~QtRateConceptDialog() noexcept
{
  delete ui;
}

void ribi::cmap::QtRateConceptDialog::DisplaySuggestions() noexcept
{
  if (boost::num_vertices(m_conceptmap) > 0)
  {
    const auto& rating = m_widget->GetRating();

    // Without vertices, there is no valid vertex descriptor
    assert(boost::num_vertices(m_conceptmap) > 0);
    const auto vd = *vertices(m_conceptmap).first;
    {
      const QString s = "Formeel uitgangspunt: "
        + QString::number(
          rating.SuggestComplexity(m_conceptmap, vd)
        );
      ui->box_complexity->setToolTip(s);
    }
    {
      const QString s = "Formeel uitgangspunt: "
        + QString::number(
          rating.SuggestConcreteness(m_conceptmap, vd)
        );
      ui->box_concreteness->setToolTip(s);
    }
    {
      const QString s = "Formeel uitgangspunt: "
        + QString::number(
          rating.SuggestSpecificity(m_conceptmap, vd)
        );
      ui->box_specificity->setToolTip(s);
    }
  }
  else
  {
    ui->box_complexity->setToolTip("Formeel uitgangspunt: -");
    ui->box_concreteness->setToolTip("Formeel uitgangspunt: -");
    ui->box_specificity->setToolTip("Formeel uitgangspunt: -");
  }
}

int ribi::cmap::QtRateConceptDialog::GetComplexity() const noexcept
{
  return ui->box_complexity->currentIndex();
}

int ribi::cmap::QtRateConceptDialog::GetConcreteness() const noexcept
{
  return ui->box_concreteness->currentIndex();
}

bool ribi::cmap::QtRateConceptDialog::GetOkClicked() const noexcept {
  return m_button_ok_clicked;
}

int ribi::cmap::QtRateConceptDialog::GetSpecificity() const noexcept
{
  return ui->box_specificity->currentIndex();
}

void ribi::cmap::QtRateConceptDialog::keyPressEvent(QKeyEvent* e)
{
  if (e->key()  == Qt::Key_Escape) { close(); return; }
  //QDialog::keyPressEvent(e);
}

void ribi::cmap::QtRateConceptDialog::on_button_ok_clicked()
{
  //Ratings already set by comboboxes
  m_button_ok_clicked = true;
  close();
}

void ribi::cmap::QtRateConceptDialog::on_button_tally_relevancies_clicked()
{
  QtRateConceptTallyDialog d(
    m_conceptmap,
    ribi::cmap::CreateDefaultRating()
  );
  d.exec(); //Keep this dialog visible, as of 2013-08-30
  ui->box_complexity->setCurrentIndex(d.GetSuggestedComplexity());
  ui->box_concreteness->setCurrentIndex(d.GetSuggestedConcreteness());
  ui->box_specificity->setCurrentIndex(d.GetSuggestedSpecificity());
}
