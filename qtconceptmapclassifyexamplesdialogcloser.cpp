#include "qtconceptmapclassifyexamplesdialogcloser.h"

#include <cassert>

#include <QApplication>
#include <QDebug>
#include <QPushButton>

#include "qtconceptmapclassifyexamplesdialog.h"
#include "ui_qtconceptmapclassifyexamplesdialog.h"

ribi::cmap::QtClassifyExamplesDialogCloser::QtClassifyExamplesDialogCloser()
{

}

ribi::cmap::QtClassifyExamplesDialogCloser::~QtClassifyExamplesDialogCloser()
{

}

void ribi::cmap::QtClassifyExamplesDialogCloser::Modify()
{
  ribi::cmap::QtClassifyExamplesDialog* const pop_up
    = qobject_cast<ribi::cmap::QtClassifyExamplesDialog*>(
      qApp->activeWindow()
    );
  assert(pop_up);
  const auto before = pop_up->GetRatedExamples();
  pop_up->ui->button_misc->click();
  if (before != pop_up->GetRatedExamples()) return;
  pop_up->ui->button_organisations->click();
  assert(before != pop_up->GetRatedExamples());
}

void ribi::cmap::QtClassifyExamplesDialogCloser::PressCancel()
{
  ribi::cmap::QtClassifyExamplesDialog* const pop_up
    = qobject_cast<ribi::cmap::QtClassifyExamplesDialog*>(
      qApp->activeWindow()
    );
  assert(pop_up);
  pop_up->ui->button_cancel->click();
  assert(pop_up->isHidden());
}

void ribi::cmap::QtClassifyExamplesDialogCloser::PressOk()
{
  ribi::cmap::QtClassifyExamplesDialog* const pop_up
    = qobject_cast<ribi::cmap::QtClassifyExamplesDialog*>(
      qApp->activeWindow()
    );
  assert(pop_up);
  pop_up->ui->button_ok->click();
  assert(pop_up->isHidden());
}
