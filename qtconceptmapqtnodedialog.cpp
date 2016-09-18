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
#include "qtconceptmapqtnodedialog.h"

#include <cassert>
#include <cmath>
#include <sstream>

#include <boost/lambda/lambda.hpp>

#include <QDesktopWidget>
#include <QVBoxLayout>

#include "conceptmapnode.h"
#include "conceptmapnodefactory.h"
#include "qtconceptmapnodedialog.h"
#include "qtconceptmapqtnodefactory.h"
#include "qtconceptmapqtnode.h"
#include "qtroundededitrectitemdialog.h"




#include "ui_qtconceptmapqtnodedialog.h"
//#include "ui_qtconceptmapnodedialog.h"
//#include "ui_qtroundededitrectitemdialog.h"
#pragma GCC diagnostic pop

ribi::cmap::QtQtNodeDialog::QtQtNodeDialog(QWidget *parent)
  : QtHideAndShowDialog(parent),
  ui(new Ui::QtQtNodeDialog),
  m_qtnode{},
  m_qtnodedialog{},
  m_qtroundededitrectitem_dialog{}
{
  ui->setupUi(this);
  {
    assert(!this->layout());
    QVBoxLayout * const my_layout{new QVBoxLayout};
    this->setLayout(my_layout);
  }
  {
    assert(this->layout());
    const boost::shared_ptr<QtNodeDialog> d{new QtNodeDialog};
    assert(d);
    m_qtnodedialog = d;
    this->layout()->addWidget(m_qtnodedialog.get());
  }
  {
    assert(this->layout());
    const boost::shared_ptr<QtRoundedEditRectItemDialog> d{new QtRoundedEditRectItemDialog};
    m_qtroundededitrectitem_dialog = d;
    this->layout()->addWidget(m_qtroundededitrectitem_dialog.get());
  }

  {
    //Put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->setGeometry(0,0,screen.width() * 9 / 10, screen.height() * 9 / 10);
    this->move( screen.center() - this->rect().center() );
  }
}

ribi::cmap::QtQtNodeDialog::~QtQtNodeDialog()
{
  delete ui;
}

int ribi::cmap::QtQtNodeDialog::GetMinimumHeight(const QtNode& qtnode) noexcept
{
  const int margin = 16;
  return
    QtNodeDialog::GetMinimumHeight(qtnode.GetNode())
  + margin
  + QtRoundedEditRectItemDialog::GetMinimumHeight(qtnode)
  ;
}

std::string ribi::cmap::QtQtNodeDialog::GetUiName() const noexcept
{
  return m_qtnodedialog->GetUiName();
}

double ribi::cmap::QtQtNodeDialog::GetUiX() const noexcept
{
  return m_qtnodedialog->GetUiX();
}

double ribi::cmap::QtQtNodeDialog::GetUiY() const noexcept
{
  return m_qtnodedialog->GetUiY();
}

void ribi::cmap::QtQtNodeDialog::SetQtNode(const boost::shared_ptr<QtNode>& qtnode) noexcept
{
  assert(qtnode);
  m_qtnode = qtnode;

  this->setMinimumHeight(
    this->GetMinimumHeight(
      *m_qtnode
    )
  );

  assert( qtnode ==  m_qtnode);
  assert(*qtnode == *m_qtnode);
}

void ribi::cmap::QtQtNodeDialog::SetUiName(const std::string& name) noexcept
{
  m_qtnodedialog->SetUiName(name);
}

void ribi::cmap::QtQtNodeDialog::SetUiX(const double x) noexcept
{
  m_qtnodedialog->SetUiX(x);
}

void ribi::cmap::QtQtNodeDialog::SetUiY(const double y) noexcept
{
  m_qtnodedialog->SetUiY(y);
}
