#include "qtconceptmapexamplesdialog.h"

#include <cassert>

#include "qtconceptmapexampledialog.h"

#include "ui_qtconceptmapexamplesdialog.h"

ribi::cmap::QtExamplesDialog::QtExamplesDialog(QWidget *parent)
  : QDialog(parent),
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
