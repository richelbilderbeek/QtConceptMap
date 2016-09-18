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
#include "qtconceptmapexampledialog.h"
#include "ui_qtconceptmapexampledialog.h"

#include <cassert>
#include <boost/bind/bind.hpp>
#include <boost/lambda/lambda.hpp>

#include "conceptmapcompetencies.h"
#include "conceptmapexample.h"
#include "conceptmapexamplefactory.h"



#pragma GCC diagnostic pop

ribi::cmap::QtExampleDialog::QtExampleDialog(QWidget *parent) :
  ribi::QtHideAndShowDialog(parent),
  ui(new Ui::QtExampleDialog),
  m_example{" ",Competency::uninitialized,false,false,false}
{
  ui->setupUi(this);
  {
    const auto v = Competencies().GetAllCompetencies();
    for (const auto c: v)
    {
      const std::string s = Competencies().ToStrDutch(c);
      ui->box_competency->addItem(s.c_str());
    }
  }

  const Example example{
    "QtExampleDialog initial example",
    Competency::uninitialized,false,false,false
  };
  this->SetExample(example);
}

ribi::cmap::QtExampleDialog::~QtExampleDialog()
{
  delete ui;
}

int ribi::cmap::QtExampleDialog::GetMinimumHeight(const Example& /*example*/) noexcept
{
  return 166;
}

void ribi::cmap::QtExampleDialog::SetExample(const Example& example)
{
  m_example = example;
  this->setMinimumHeight(GetMinimumHeight(m_example));
  assert(example == m_example);
}

void ribi::cmap::QtExampleDialog::on_box_competency_currentIndexChanged(int index)
{
  assert(index >= 0);
  assert(index < static_cast<int>(Competencies().GetAllCompetencies().size()));
  const auto competency = Competencies().GetAllCompetencies()[index];

  //Let the Example figure out itself if this changes anything;
  //Allow setting a new competency if it equals the current
  m_example.SetCompetency(competency);

  assert(m_example.GetCompetency() == competency);
}

void ribi::cmap::QtExampleDialog::on_box_is_complex_stateChanged(int)
{
  m_example.SetIsComplex(ui->box_is_complex->isChecked());
}

void ribi::cmap::QtExampleDialog::on_box_is_concrete_stateChanged(int)
{
  m_example.SetIsConcrete(ui->box_is_concrete->isChecked());
}

void ribi::cmap::QtExampleDialog::on_box_is_specific_stateChanged(int)
{
  m_example.SetIsSpecific(ui->box_is_specific->isChecked());
}

void ribi::cmap::QtExampleDialog::on_edit_text_textChanged(const QString &arg1)
{
  m_example.SetText(arg1.toStdString());
}
