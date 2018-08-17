#include "qtconceptmaprateconcepttallydialogcloser.h"

#include <cassert>

#include <QApplication>
#include <QPushButton>

#include "qtconceptmaprateconcepttallydialog.h"

ribi::cmap::QtRateConceptTallyDialogCloser::QtRateConceptTallyDialogCloser(QPushButton * const button)
  : m_button{button}
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
  m_button->click();
  assert(pop_up->isHidden());
}
