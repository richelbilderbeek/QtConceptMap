
/*














*/



#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "qtconceptmapnodedialog.h"

#include <cassert>
#include <boost/bind/bind.hpp>
#include <boost/lambda/lambda.hpp>

#include "conceptmapcompetencies.h"
#include "conceptmapconcept.h"
#include "conceptmapconceptfactory.h"
#include "conceptmapexamples.h"
#include "conceptmapnode.h"
#include "conceptmapnodefactory.h"
#include "qtconceptmapconceptdialog.h"
#include "qtconceptmapexamplesdialog.h"
#include "ui_qtconceptmapnodedialog.h"



#pragma GCC diagnostic pop

ribi::cmap::QtNodeDialog::QtNodeDialog(QWidget *parent)
  : ribi::QtHideAndShowDialog(parent),
    ui(new Ui::QtNodeDialog),
    m_node{},
    m_qtconceptdialog{new QtConceptDialog}
{
  ui->setupUi(this);
  {
    assert(layout());
    layout()->addWidget(m_qtconceptdialog.get());

  }
}

ribi::cmap::QtNodeDialog::~QtNodeDialog()
{
  delete ui;
}

int ribi::cmap::QtNodeDialog::GetMinimumHeight(const Node& node) noexcept
{
  return QtConceptDialog::GetMinimumHeight(node.GetConcept()) + 82;
}


double ribi::cmap::QtNodeDialog::GetUiX() const noexcept
{
  return ui->box_x->value();
}

double ribi::cmap::QtNodeDialog::GetUiY() const noexcept
{
  return ui->box_y->value();
}

std::string ribi::cmap::QtNodeDialog::GetUiName() const noexcept
{
  return m_qtconceptdialog->GetUiName();
}

void ribi::cmap::QtNodeDialog::SetNode(const Node& node) noexcept
{
  m_node = node;

  this->setMinimumHeight(
    this->GetMinimumHeight(
      m_node
    )
  );
  assert(node ==  m_node);
}

void ribi::cmap::QtNodeDialog::SetUiName(const std::string& name) noexcept
{
  this->m_qtconceptdialog->SetUiName(name);
  assert(GetUiName() == name);
}


void ribi::cmap::QtNodeDialog::SetUiX(const double x) noexcept
{
  ui->box_x->setValue(x);
  //Calls 'on_box_x_valueChanged' automatically
}

void ribi::cmap::QtNodeDialog::SetUiY(const double y) noexcept
{
  ui->box_y->setValue(y);
  //Calls 'on_box_y_valueChanged' automatically
  //ui->box_y does not immediatly update
  //m_node.SetY(y);
  //on_box_y_valueChanged(y);
}

void ribi::cmap::QtNodeDialog::on_box_x_valueChanged(double arg1)
{
  m_node.SetX(arg1);
}

void ribi::cmap::QtNodeDialog::on_box_y_valueChanged(double arg1)
{
  m_node.SetY(arg1);
}
