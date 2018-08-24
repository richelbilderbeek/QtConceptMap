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

void ribi::cmap::QtRateConceptDialogCloser::PressCancel()
{
  ribi::cmap::QtRateConceptDialog* const pop_up
    = qobject_cast<ribi::cmap::QtRateConceptDialog*>(
      qApp->activeWindow()
    );
  assert(pop_up);
  pop_up->ui->button_cancel->click();
  assert(pop_up->isHidden());
}

void ribi::cmap::QtRateConceptDialogCloser::PressOk()
{
  ribi::cmap::QtRateConceptDialog* const pop_up
    = qobject_cast<ribi::cmap::QtRateConceptDialog*>(
      qApp->activeWindow()
    );
  assert(pop_up);
  pop_up->ui->button_ok->click();
  assert(pop_up->isHidden());
}
