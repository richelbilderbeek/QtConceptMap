//---------------------------------------------------------------------------
/*
QtConceptMap, Qt classes for display and interaction with ConceptMap
Copyright (C) 2013-2016 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/CppQtConceptMap.htm
//---------------------------------------------------------------------------
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "qtconceptmaprateconceptdialognewname.h"

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
//#include "conceptmapfile.h"
#include "conceptmapnodefactory.h"
#include "conceptmapnode.h"
#include "qtconceptmaprating.h"
#include "qtconceptmap.h"
#include "qtconceptmaprateconcepttallydialognewname.h"
#include "trace.h"
#include "ui_qtconceptmaprateconceptdialognewname.h"
#pragma GCC diagnostic pop


ribi::cmap::QtRateConceptDialog::QtRateConceptDialog(
  const ConceptMap conceptmap,
  QWidget* parent)
  : QtHideAndShowDialog(parent),
    ui(new Ui::QtRateConceptDialog),
    m_button_ok_clicked(false),
    m_conceptmap(conceptmap),
    m_widget(new QtConceptMap)
{
  ui->setupUi(this);
  m_widget->SetConceptMap(conceptmap);
  assert(boost::num_vertices(m_conceptmap));

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
  {
    const std::string s = "Formeel uitgangspunt: "
      + boost::lexical_cast<std::string>(
        cmap::Rating().SuggestComplexity(m_conceptmap,*vertices(m_conceptmap).first)
      );
    ui->box_complexity->setToolTip(s.c_str());
  }
  {
    const std::string s = "Formeel uitgangspunt: "
      + boost::lexical_cast<std::string>(
        cmap::Rating().SuggestConcreteness(m_conceptmap,*vertices(m_conceptmap).first)
      );
    ui->box_concreteness->setToolTip(s.c_str());
  }
  {
    const std::string s = "Formeel uitgangspunt: "
      + boost::lexical_cast<std::string>(
        cmap::Rating().SuggestSpecificity(m_conceptmap,*vertices(m_conceptmap).first)
      );
    ui->box_specificity->setToolTip(s.c_str());
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
  QtRateConceptTallyDialog d(m_conceptmap);
  d.exec(); //Keep this dialog visible, as of 2013-08-30
  ui->box_complexity->setCurrentIndex(d.GetSuggestedComplexity());
  ui->box_concreteness->setCurrentIndex(d.GetSuggestedConcreteness());
  ui->box_specificity->setCurrentIndex(d.GetSuggestedSpecificity());
}
