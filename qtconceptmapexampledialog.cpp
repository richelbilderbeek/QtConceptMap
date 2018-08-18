#include "qtconceptmapexampledialog.h"

#ifdef REALLY_NEED_THIS_20180818

#include <cassert>

#include "conceptmapcompetencies.h"
#include "ui_qtconceptmapexampledialog.h"

ribi::cmap::QtExampleDialog::QtExampleDialog(QWidget *parent) :
  QDialog(parent),
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

int ribi::cmap::QtExampleDialog::GetMinimumHeight(const Example&) noexcept
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

#endif // REALLY_NEED_THIS_20180818
