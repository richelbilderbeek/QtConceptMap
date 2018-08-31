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

void ribi::cmap::QtEditConceptDialogCloser::PressCancel()
{
  QtEditConceptDialog* const pop_up
    = qobject_cast<QtEditConceptDialog*>(
      qApp->activeWindow()
    );
  assert(pop_up);
  pop_up->ui->button_cancel->click();
  assert(pop_up->isHidden());
}

void ribi::cmap::QtEditConceptDialogCloser::PressOk()
{
  QtEditConceptDialog* const pop_up
    = qobject_cast<QtEditConceptDialog*>(
      qApp->activeWindow()
    );
  assert(pop_up);
  pop_up->ui->button_ok->click();
  assert(pop_up->isHidden());
}
