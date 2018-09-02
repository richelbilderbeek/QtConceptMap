#include "qtconceptmaprateconceptdialogcloser.h"

#include <cassert>

#include <QApplication>
#include <QDebug>
#include <QPushButton>

#include "qtconceptmaprateconceptdialog.h"
#include "ui_qtconceptmaprateconceptdialog.h"

ribi::cmap::QtRateConceptDialogCloser::QtRateConceptDialogCloser()
{

}

ribi::cmap::QtRateConceptDialogCloser::~QtRateConceptDialogCloser()
{

}

ribi::cmap::QtRateConceptDialog * ribi::cmap::QtRateConceptDialogCloser::GetDialog() const noexcept
{
  ribi::cmap::QtRateConceptDialog* pop_up = nullptr;
  while (!pop_up)
  {
    pop_up
      = qobject_cast<ribi::cmap::QtRateConceptDialog*>(
        qApp->activeWindow()
      );
    qApp->processEvents();
  }
  assert(pop_up);
  return pop_up;
}

void ribi::cmap::QtRateConceptDialogCloser::Modify()
{
  auto * const pop_up = GetDialog();
  assert(pop_up);
  pop_up->ui->box_complexity->setCurrentIndex(
    (pop_up->ui->box_complexity->currentIndex() + 1)
    % pop_up->ui->box_complexity->count()
  );
  pop_up->ui->box_concreteness->setCurrentIndex(
    (pop_up->ui->box_concreteness->currentIndex() + 1)
    % pop_up->ui->box_concreteness->count()
  );
  pop_up->ui->box_specificity->setCurrentIndex(
    (pop_up->ui->box_specificity->currentIndex() + 1)
    % pop_up->ui->box_specificity->count()
  );
}

void ribi::cmap::QtRateConceptDialogCloser::ModifyAndOk()
{
  Modify();
  PressOk();
}

void ribi::cmap::QtRateConceptDialogCloser::PressCancel()
{
  auto * const pop_up = GetDialog();
  assert(pop_up);
  pop_up->ui->button_cancel->click();
  assert(pop_up->isHidden());
}

void ribi::cmap::QtRateConceptDialogCloser::PressOk()
{
  auto * const pop_up = GetDialog();
  assert(pop_up);
  pop_up->ui->button_ok->click();
  assert(pop_up->isHidden());
}
