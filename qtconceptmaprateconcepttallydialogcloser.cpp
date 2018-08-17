#include "qtconceptmaprateconcepttallydialogcloser.h"

#include <cassert>

#include <QApplication>
#include <QDebug>
#include <QPushButton>

#include "qtconceptmaprateconcepttallydialog.h"
#include "ui_qtconceptmaprateconcepttallydialog.h"

ribi::cmap::QtRateConceptTallyDialogCloser::QtRateConceptTallyDialogCloser()
{

}

ribi::cmap::QtRateConceptTallyDialogCloser::~QtRateConceptTallyDialogCloser()
{

}

void ribi::cmap::QtRateConceptTallyDialogCloser::Close()
{
  ribi::cmap::QtRateConceptTallyDialog* const pop_up
    = qobject_cast<ribi::cmap::QtRateConceptTallyDialog*>(
      qApp->activeWindow()
    );
  assert(pop_up);
  pop_up->ui->button_ok->click();
  assert(pop_up->isHidden());
}
