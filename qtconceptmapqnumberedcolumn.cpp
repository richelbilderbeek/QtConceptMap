#include "qtconceptmapqnumberedcolumn.h"

#include <QApplication>
#include <QKeyEvent>
#include <QTableWidgetItem>
#include <QHeaderView>

ribi::cmap::QNumberedColumn::QNumberedColumn(
  const QVector<QString>& text,
  QWidget *parent
) : QTableWidget(text.size(), 1, parent)
{
  this->setAlternatingRowColors(true);
  this->setTextElideMode(Qt::ElideNone);
  this->horizontalHeader()->setStretchLastSection(true);
  this->horizontalHeader()->setSizeAdjustPolicy(QHeaderView::AdjustToContents);
  this->horizontalHeader()->setHidden(true);
  this->verticalHeader()->setSizeAdjustPolicy(QHeaderView::AdjustToContents);

  const int n_lines{text.size()};
  for (int i = 0; i != n_lines; ++i)
  {
    QTableWidgetItem * const item = new QTableWidgetItem(text[i]);
    this->setItem(i, 0, item);
  }
  connect(
   this->horizontalHeader(),
    SIGNAL(sectionResized(int, int, int)),
    this,
    SLOT(resizeRowsToContents())
  );

  connect(
    this,
    SIGNAL(cellChanged(int,int)),
    this,
    SLOT(OnCellChanged(int,int))
  );
}

ribi::cmap::QNumberedColumn::~QNumberedColumn()
{

}

void ribi::cmap::QNumberedColumn::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Tab)
  {
    this->nextInFocusChain()->setFocus();
    event->accept();
    return;
  }
  QTableWidget::keyPressEvent(event);
}

void ribi::cmap::QNumberedColumn::OnCellChanged(int row, int column)
{
  if (this->item(row, column)->text().isEmpty())
  {
    this->removeRow(row);
  }
}
