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
#include "qtconceptmapedgedialog.h"

#include <cassert>
#include <boost/bind/bind.hpp>
#include <boost/lambda/lambda.hpp>

#include <QLabel>

#include "conceptmapedge.h"
#include "conceptmapedgefactory.h"
#include "conceptmapnode.h"
#include "conceptmapnodefactory.h"
#include "conceptmapconcept.h"
#include "conceptmapconceptfactory.h"
#include "qtconceptmapexampledialog.h"
#include "qtconceptmapexamplesdialog.h"
#include "qtconceptmapconceptdialog.h"
#include "qtconceptmapnodedialog.h"
#include "ui_qtconceptmapedgedialog.h"



#pragma GCC diagnostic pop

ribi::cmap::QtEdgeDialog::QtEdgeDialog(
  const Edge& edge,
  QWidget *parent
)
  : ribi::QtHideAndShowDialog(parent),
    ui(new Ui::QtEdgeDialog),
    m_edge{edge},
    m_qtnodedialog{new QtNodeDialog},
    m_qtnodedialog_from{new QtNodeDialog},
    m_qtnodedialog_to{new QtNodeDialog}
{
  ui->setupUi(this);
  setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint); //Remove help

  {
    assert(ui->groupBox->layout());
    const auto my_layout = ui->groupBox->layout();
    {
      QLabel * const label = new QLabel("Center node:");
      my_layout->addWidget(label);
    }
    my_layout->addWidget(m_qtnodedialog.get());
    {
      QLabel * const label = new QLabel("From node:");
      my_layout->addWidget(label);
    }
    my_layout->addWidget(m_qtnodedialog_from.get());
    {
      QLabel * const label = new QLabel("To node:");
      my_layout->addWidget(label);
    }
    my_layout->addWidget(m_qtnodedialog_to.get());
  }
}

ribi::cmap::QtEdgeDialog::~QtEdgeDialog()
{
  delete ui;
}

int ribi::cmap::QtEdgeDialog::GetMinimumHeight(const Edge& ) noexcept
{
  return 400;
}


bool ribi::cmap::QtEdgeDialog::GetUiHasHeadArrow() const noexcept
{
  return ui->box_head_arrow->isChecked();
}

bool ribi::cmap::QtEdgeDialog::GetUiHasTailArrow() const noexcept
{
  return ui->box_tail_arrow->isChecked();
}

double ribi::cmap::QtEdgeDialog::GetUiX() const noexcept
{
  return m_qtnodedialog->GetUiX();
}

double ribi::cmap::QtEdgeDialog::GetUiY() const noexcept
{
  return m_qtnodedialog->GetUiY();
}

void ribi::cmap::QtEdgeDialog::SetEdge(const Edge& edge)
{
  m_edge = edge;

  setMinimumHeight(GetMinimumHeight(m_edge));

  assert(edge == m_edge);
}

void ribi::cmap::QtEdgeDialog::SetUiHasHeadArrow(const bool has_head) noexcept
{
  ui->box_head_arrow->setChecked(has_head);
}

void ribi::cmap::QtEdgeDialog::SetUiHasTailArrow(const bool has_tail) noexcept
{
  ui->box_tail_arrow->setChecked(has_tail);
}

void ribi::cmap::QtEdgeDialog::SetUiX(const double x) noexcept
{
  m_qtnodedialog->SetUiX(x);
}


void ribi::cmap::QtEdgeDialog::SetUiY(const double y) noexcept
{
  m_qtnodedialog->SetUiY(y);
}
