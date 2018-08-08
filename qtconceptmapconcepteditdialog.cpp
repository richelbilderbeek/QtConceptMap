#include "qtconceptmapconcepteditdialog.h"

#include <iostream>
#include <cassert>
#include <QScrollBar>
#include <QTableView>
#include <QLayout>
#include <QVBoxLayout>

#include "conceptmapexample.h"
#include "qtconceptmapqnumberedcolumn.h"
#include "ui_qtconceptmapconcepteditdialog.h"

ribi::cmap::QtConceptMapConceptEditDialog::QtConceptMapConceptEditDialog(
  const Concept& c,
  const EditType edit_type,
  QWidget* parent)
  : QDialog(parent),
    m_examples{new QNumberedColumn(GetExamplesText(c), this)},
    ui(new Ui::QtConceptMapConceptEditDialog)
{
  ui->setupUi(this);

  if (edit_type == EditType::concept)
  {
    this->setWindowTitle("Concept bewerken");
    ui->label_concept_or_relation->setText("Concept");
  }
  if (edit_type == EditType::relation)
  {
    this->setWindowTitle("Relatie bewerken");
    ui->label_concept_or_relation->setText("Relatie");
  }
  //Convert the concept to its GUI elements
  ui->edit_concept->setPlainText(c.GetName().c_str());
  ui->edit_concept->setTabChangesFocus(true);

  //Add the examples
  ui->verticalLayout->insertWidget(3, m_examples);

  //Prepare text
  ui->edit_text->clear();
  ui->edit_text->setTabChangesFocus(true);
  ui->edit_concept->setFocus();

  connect(
    ui->button_ok,
    SIGNAL(clicked(bool)),
    this,
    SLOT(close())
  );
}

ribi::cmap::QtConceptMapConceptEditDialog::~QtConceptMapConceptEditDialog()
{
  delete ui;
}

QVector<QString> ribi::cmap::GetExamplesText(const Concept& concept) noexcept
{
  QVector<QString> text;
  for (const Example& example: GetExamples(concept).Get())
  {
    text.push_back(QString::fromStdString(example.GetText()));
  }
  return text;
}

void ribi::cmap::QtConceptMapConceptEditDialog::keyPressEvent(QKeyEvent* e)
{
  if (e->key() == Qt::Key_Escape) { close(); return; }
  //No tabs allowed in editing
  //if (e->key() == Qt::Key_Tab)
  //{
  //  QDialog::keyPressEvent(e);
  //  return;
  //}
  if (
    (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)
    && e->modifiers() & Qt::AltModifier
  )
  {
    close();
    return;
  }

  QDialog::keyPressEvent(e); //Causes dialog to close unwanted?
}

void ribi::cmap::QtConceptMapConceptEditDialog::on_button_add_clicked()//QTableView *tableView)
{
  auto * const new_item = new QTableWidgetItem(
    ui->edit_text->toPlainText()
  );
  assert(m_examples);
  const int cur_row_count = m_examples->rowCount();

  m_examples->setRowCount(cur_row_count + 1);
  m_examples->setItem(cur_row_count, 0, new_item);
  m_examples->scrollToBottom();

  ui->edit_text->clear();
  ui->edit_text->setFocus();
}

void ribi::cmap::QtConceptMapConceptEditDialog
  ::RemoveEmptyItem(QTableWidgetItem * item)
{
  if (item->text().isEmpty())
  {
    m_examples->removeRow(item->row());
    this->update();
  }
}

ribi::cmap::Concept ribi::cmap::QtConceptMapConceptEditDialog::ToConcept() const noexcept
{
  //Name
  const std::string name = ui->edit_concept->toPlainText().toStdString();
  //Examples
  std::vector<Example> v;

  const int n_items = m_examples->rowCount();
  for (int i=0; i != n_items; ++i)
  {
    const auto* const item = m_examples->item(i, 0);
    const Example p(
      item->text().toStdString()
    );
    v.push_back(p);
  }
  assert(n_items == static_cast<int>(v.size()));
  return Concept(name, Examples(v));
}
