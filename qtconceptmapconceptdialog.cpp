



#include "qtconceptmapconceptdialog.h"

#include <cassert>
#include <boost/bind/bind.hpp>
#include <boost/lambda/lambda.hpp>

#include "conceptmapcompetencies.h"
#include "conceptmapconcept.h"
#include "conceptmapconceptfactory.h"
#include "conceptmapexamples.h"
#include "conceptmapexamplesfactory.h"
#include "qtconceptmapexamplesdialog.h"

#include "ui_qtconceptmapconceptdialog.h"



ribi::cmap::QtConceptDialog::QtConceptDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::QtConceptDialog),
  m_concept{ConceptFactory().Create()},
  m_qtexamplesdialog{new QtExamplesDialog(this)}
{
  ui->setupUi(this);
  {
    assert(layout());
    layout()->addWidget(m_qtexamplesdialog);

  }

  const Concept concept{
    "QtConceptDialog initial concept",
    ExamplesFactory().GetTest(2),true,-1,-1,-1
  };
  this->SetConcept(concept);
}

ribi::cmap::QtConceptDialog::~QtConceptDialog() noexcept
{
  delete ui;
}

int ribi::cmap::QtConceptDialog::GetMinimumHeight(const Concept& concept) noexcept
{
  return
      QtExamplesDialog::GetMinimumHeight(concept.GetExamples())
    + 197
  ;
}

std::string ribi::cmap::QtConceptDialog::GetUiName() const noexcept
{
  return ui->edit_name->text().toStdString();
}

void ribi::cmap::QtConceptDialog::SetConcept(const Concept& concept) noexcept
{
  m_concept = concept;

  setMinimumHeight(GetMinimumHeight(m_concept));

  assert(concept ==  m_concept);
}

void ribi::cmap::QtConceptDialog::SetUiName(const std::string& name) noexcept
{
  ui->edit_name->setText(name.c_str());
  assert(GetUiName() == name);
}

void ribi::cmap::QtConceptDialog::on_box_is_complex_stateChanged(int) noexcept
{
  m_concept.SetIsComplex(ui->box_is_complex->isChecked());
}

void ribi::cmap::QtConceptDialog::on_edit_name_textChanged(const QString &arg1) noexcept
{
  m_concept.SetName(arg1.toStdString());
}

void ribi::cmap::QtConceptDialog::on_box_rating_complexity_valueChanged(int arg1) noexcept
{
  m_concept.SetRatingComplexity(arg1);
}

void ribi::cmap::QtConceptDialog::on_box_rating_concreteness_valueChanged(int arg1) noexcept
{
  m_concept.SetRatingConcreteness(arg1);
}

void ribi::cmap::QtConceptDialog::on_box_rating_specificity_valueChanged(int arg1) noexcept
{
  m_concept.SetRatingSpecificity(arg1);
}
