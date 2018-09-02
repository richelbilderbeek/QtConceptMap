#include "qtconceptmapeditconceptdialogcloser.h"

#include <cassert>

#include <QApplication>
#include <QDebug>
#include <QPushButton>

#include "qtconceptmapeditconceptdialog.h"
#include "ui_qtconceptmapeditconceptdialog.h"

ribi::cmap::QtEditConceptDialogCloser::QtEditConceptDialogCloser()
{

}

ribi::cmap::QtEditConceptDialogCloser::~QtEditConceptDialogCloser()
{

}

ribi::cmap::QtEditConceptDialog * ribi::cmap::QtEditConceptDialogCloser::GetDialog() const noexcept
{
  QtEditConceptDialog* pop_up = nullptr;
  while (!pop_up)
  {
    pop_up = qobject_cast<QtEditConceptDialog*>(
      qApp->activeWindow()
    );
    qApp->processEvents();
  }
  return pop_up;
}

void ribi::cmap::QtEditConceptDialogCloser::ModifyAndOk()
{
  Modify();
  PressOk();
}

void ribi::cmap::QtEditConceptDialogCloser::Modify()
{
  auto * const pop_up = GetDialog();
  assert(pop_up);
  pop_up->ui->edit_text->setPlainText(
    pop_up->ui->edit_text->toPlainText() + "!"
  );
  pop_up->ui->edit_concept->setPlainText(
    pop_up->ui->edit_concept->toPlainText() + "!"
  );
  pop_up->ui->button_add->click();
}

void ribi::cmap::QtEditConceptDialogCloser::PressCancel()
{
  auto * const pop_up = GetDialog();
  assert(pop_up);
  pop_up->ui->button_cancel->click();
  assert(pop_up->isHidden());
}

void ribi::cmap::QtEditConceptDialogCloser::PressOk()
{
  auto * const pop_up = GetDialog();
  assert(pop_up);
  pop_up->ui->button_ok->click();
  assert(pop_up->isHidden());
}
