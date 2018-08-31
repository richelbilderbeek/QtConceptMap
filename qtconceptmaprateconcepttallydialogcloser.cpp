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

void ribi::cmap::QtRateConceptTallyDialogCloser::Modify()
{
  ribi::cmap::QtRateConceptTallyDialog* const pop_up
    = qobject_cast<ribi::cmap::QtRateConceptTallyDialog*>(
      qApp->activeWindow()
    );
  assert(pop_up);
  auto * const table = pop_up->ui->table;
  const int n_rows{table->rowCount()};
  const int n_cols{table->columnCount()};
  for (int row = 0; row != n_rows; ++row)
  {
    for (int col = 0; col != n_cols; ++col)
    {
      auto * const item = table->item(row, col);
      if (!item) continue;
      if (item->checkState() == Qt::CheckState::Unchecked)
      {
        item->setCheckState(Qt::CheckState::Checked);
      }
      else if (item->checkState() == Qt::CheckState::Checked)
      {
        item->setCheckState(Qt::CheckState::Unchecked);
      }
    }
  }
}

void ribi::cmap::QtRateConceptTallyDialogCloser::PressCancel()
{
  ribi::cmap::QtRateConceptTallyDialog* const pop_up
    = qobject_cast<ribi::cmap::QtRateConceptTallyDialog*>(
      qApp->activeWindow()
    );
  assert(pop_up);
  pop_up->ui->button_cancel->click();
  assert(pop_up->isHidden());
}

void ribi::cmap::QtRateConceptTallyDialogCloser::PressOk()
{
  ribi::cmap::QtRateConceptTallyDialog* const pop_up
    = qobject_cast<ribi::cmap::QtRateConceptTallyDialog*>(
      qApp->activeWindow()
    );
  assert(pop_up);
  pop_up->ui->button_ok->click();
  assert(pop_up->isHidden());
}
