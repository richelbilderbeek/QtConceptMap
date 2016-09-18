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

#pragma GCC diagnostic pop

ribi::cmap::QtConceptDialog::QtConceptDialog(QWidget *parent) :
  ribi::QtHideAndShowDialog(parent),
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
