
/*














*/



#include "qtconceptmapexamplesdialog.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <cassert>

#include <boost/bind/bind.hpp>
#include <boost/lambda/lambda.hpp>

#include <QLabel>
#include <QLayout>
#include <QVBoxLayout>

#include "conceptmapexample.h"
#include "conceptmapexamples.h"
#include "conceptmapexamplesfactory.h"
#include "qtconceptmapexampledialog.h"


#include "ui_qtconceptmapexamplesdialog.h"
#pragma GCC diagnostic pop

ribi::cmap::QtExamplesDialog::QtExamplesDialog(QWidget *parent)
  : QtHideAndShowDialog(parent),
    ui(new Ui::QtExamplesDialog),
    m_dialogs{},
    m_examples{}
{
  ui->setupUi(this);
}

ribi::cmap::QtExamplesDialog::~QtExamplesDialog()
{
  delete ui;
}

int ribi::cmap::QtExamplesDialog::GetMinimumHeight(const Examples& examples) noexcept
{
  int height = 0;
  for (const auto example: examples.Get())
  {
    const int margin = 16;
    height += QtExampleDialog::GetMinimumHeight(example);
    height += margin;
  }
  return height;
}

void ribi::cmap::QtExamplesDialog::SetExamples(const Examples& examples)
{
  m_examples = examples;
  assert( m_examples ==  examples);
  this->setMinimumHeight(GetMinimumHeight(m_examples));
}
