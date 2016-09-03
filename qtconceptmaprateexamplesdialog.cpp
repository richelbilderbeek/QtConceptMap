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
#pragma GCC diagnostic pop

struct QtConceptMapListWidgetItem : public QListWidgetItem
{
  QtConceptMapListWidgetItem(const ribi::cmap::Example& example)
  {
    //Wordwrap text
    const std::string s = example.GetText();
    const std::vector<std::string> v { ribi::cmap::Wordwrap(s,40) };
    //Display multi-line
    std::string t;
    for (const std::string& i: v) { t += i + "\n"; }
    if (!t.empty()) t.resize(t.size() - 1);
    this->setText(t.c_str());
    //Icon
    this->setIcon(ribi::cmap::QtCompetency().CompetencyToIcon(example.GetCompetency()));
  }
};


ribi::cmap::QtRateExamplesDialogNewName::QtRateExamplesDialogNewName(
  const Concept& concept,
  QWidget* parent)
  : QtHideAndShowDialog(parent),
    ui(new Ui::QtRateExamplesDialogNewName),
    m_concept(concept),
    m_concept_at_start(concept)
{
  ui->setupUi(this);

  //Ensure that the dialog does not resize beyond the screen's size
  {
    this->setMaximumWidth( QApplication::desktop()->width() );
  }

  //Convert the Concept to GUI elements
  {
    ui->label_concept_name->setText(concept.GetName().c_str());
    ui->list->clear();
    const auto v = concept.GetExamples().Get();
    const std::size_t sz = v.size();
    for (std::size_t i=0; i!=sz; ++i)
    {
      const Example& example = v[i];
      QtConceptMapListWidgetItem * const item
        = new QtConceptMapListWidgetItem(example);
      ui->list->addItem(item);
    }
  }
  //Align the button icons and texts to the left
  {
    const std::vector<QPushButton*> v
      =
      {
        ui->button_misc,
        ui->button_organisations,
        ui->button_prof,
        ui->button_prof_development,
        ui->button_social,
        ui->button_target_audience,
        ui->button_ti_knowledge
      };
    for(QPushButton* button: v) button->setStyleSheet("text-align: left;");
  }

  //Put the dialog in the screen its center
  {
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );
  }

}

ribi::cmap::QtRateExamplesDialogNewName::~QtRateExamplesDialogNewName() noexcept
{
  delete ui;
}

ribi::cmap::Examples ribi::cmap::QtRateExamplesDialogNewName::GetRatedExamples() const
{
  std::vector<Example> v;

  const int sz = ui->list->count();
  for (int i=0; i!=sz; ++i)
  {
    const QListWidgetItem* const qtitem = ui->list->item(i);
    assert(qtitem);
    const QtConceptMapListWidgetItem* const item
      = dynamic_cast<const QtConceptMapListWidgetItem*>(qtitem);
    assert(item);
    const Example example{
      item->text().toStdString(),
      QtCompetency().IconToCompetency(item->icon())
    };
    v.push_back(example);
  }

  return Examples(v);
}

void ribi::cmap::QtRateExamplesDialogNewName::keyPressEvent(QKeyEvent* e)
{
  if (e->key()  == Qt::Key_Escape) { close(); return; }
  if ( (e->modifiers() & Qt::ControlModifier)
    && (e->modifiers() & Qt::ShiftModifier)
    && e->key() == Qt::Key_T)
  {
    //Translate
    this->setWindowTitle("Classification screen");
    ui->button_misc->setText("Other knowledge");
    ui->button_ok->setText("OK");
    ui->button_organisations->setText("Organisational knowledge");
    ui->button_prof->setText("Vocational domain knowledge");
    ui->button_prof_development->setText("Knowledge relevant for personal development");
    ui->button_social->setText("Knowledge of ones own social environment");
    ui->button_target_audience->setText("Target group knowledge");
    ui->button_ti_knowledge->setText("Technical-instrumental knowledge");
    ui->label_above_list->setText("Illustrations");
    //ui->label_concept_name->setText(""); Read from concept map
    ui->groupBox->setTitle("Mark text");
    return;
  }
  QDialog::keyPressEvent(e);
}

void ribi::cmap::QtRateExamplesDialogNewName::on_button_prof_clicked()
{
  if (ui->list->currentItem())
  {
    QtConceptMapListWidgetItem* const item
     = dynamic_cast<QtConceptMapListWidgetItem*>(ui->list->currentItem());
    item->setIcon(cmap::QtCompetency().CompetencyToIcon(cmap::Competency::profession));
  }
}

void ribi::cmap::QtRateExamplesDialogNewName::on_button_organisations_clicked()
{
  if (ui->list->currentItem())
  {
    QtConceptMapListWidgetItem* const item
     = dynamic_cast<QtConceptMapListWidgetItem*>(ui->list->currentItem());
    item->setIcon(cmap::QtCompetency().CompetencyToIcon(cmap::Competency::organisations));
  }
}

void ribi::cmap::QtRateExamplesDialogNewName::on_button_social_clicked()
{
  if (ui->list->currentItem())
  {
    QtConceptMapListWidgetItem* const item
     = dynamic_cast<QtConceptMapListWidgetItem*>(ui->list->currentItem());
    item->setIcon(cmap::QtCompetency().CompetencyToIcon(cmap::Competency::social_surroundings));
  }
}

void ribi::cmap::QtRateExamplesDialogNewName::on_button_target_audience_clicked()
{
  if (ui->list->currentItem())
  {
    QtConceptMapListWidgetItem* const item
     = dynamic_cast<QtConceptMapListWidgetItem*>(ui->list->currentItem());
    item->setIcon(cmap::QtCompetency().CompetencyToIcon(cmap::Competency::target_audience));
  }
}

void ribi::cmap::QtRateExamplesDialogNewName::on_button_prof_development_clicked()
{
  if (ui->list->currentItem())
  {
    QtConceptMapListWidgetItem* const item
     = dynamic_cast<QtConceptMapListWidgetItem*>(ui->list->currentItem());
    item->setIcon(cmap::QtCompetency().CompetencyToIcon(cmap::Competency::prof_growth));
  }
}

void ribi::cmap::QtRateExamplesDialogNewName::on_button_misc_clicked()
{
  if (ui->list->currentItem())
  {
    QtConceptMapListWidgetItem* const item
      = dynamic_cast<QtConceptMapListWidgetItem*>(ui->list->currentItem());
    item->setIcon(cmap::QtCompetency().CompetencyToIcon(cmap::Competency::misc));
  }
}

void ribi::cmap::QtRateExamplesDialogNewName::on_button_ti_knowledge_clicked()
{
  if (ui->list->currentItem())
  {
    QtConceptMapListWidgetItem* const item
      = dynamic_cast<QtConceptMapListWidgetItem*>(ui->list->currentItem());
    item->setIcon(cmap::QtCompetency().CompetencyToIcon(cmap::Competency::ti_knowledge));
  }
}

void ribi::cmap::QtRateExamplesDialogNewName::on_button_ok_clicked()
{
  const Examples p = GetRatedExamples();
  m_concept.SetExamples(p);
  close();
}
