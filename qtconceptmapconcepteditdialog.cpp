#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "qtconceptmapconcepteditdialog.h"

#include <cassert>

#include <QKeyEvent>
#include <QObjectList>
#include <QListWidgetItem>
#include <QTableWidgetItem>
#include <QTableWidget>

#include "conceptmapcompetency.h"
#include "conceptmapexample.h"
#include "conceptmapexamplefactory.h"
#include "conceptmapexamplesfactory.h"
#include "conceptmapexamples.h"
#include "conceptmaphelper.h"
#include "conceptmapconcept.h"
#include "conceptmapconceptfactory.h"
#include "qtconceptmapcompetency.h"
#include "ui_qtconceptmapconcepteditdialog.h"

#pragma GCC diagnostic pop

///QTreeWidgetItem with the only function of storing a
///cmap::Competency additionally, only used in testing
struct QtConceptMapListWidgetItem : public QListWidgetItem
{
  QtConceptMapListWidgetItem(
    const ribi::cmap::Competency competency
    )
    : QListWidgetItem(0),
      m_competency(competency)
  {

  }
  const ribi::cmap::Competency m_competency;
};

ribi::cmap::QtConceptMapConceptEditDialog::QtConceptMapConceptEditDialog(
  const Concept& concept,
  QWidget* parent)
  : QtHideAndShowDialog(parent),
    ui(new Ui::QtConceptMapConceptEditDialog)
{
  ui->setupUi(this);

  //Convert the concept to its GUI elements
  //Add the name
  ui->edit_concept->setPlainText(concept.GetName().c_str());
  //Add the examples
  const auto& examples = concept.GetExamples().Get();
  const int n_examples = examples.size();
  ui->list_examples->setWordWrap(true);
  ui->list_examples->setRowCount(n_examples);
  for (int i = 0; i != n_examples; ++i)
  {
    const auto& example = examples[i];
    QTableWidgetItem * const item = new QTableWidgetItem(example.GetText().c_str());
    ui->list_examples->setItem(i, 0, item);
  }
  assert(ui->list_examples->verticalHeader());
  if (n_examples)
  {
    ui->list_examples->verticalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
  }
  assert(ui->list_examples->isEnabled());
  QObject::connect(
    ui->list_examples,
    SIGNAL(itemChanged(QListWidgetItem*)),
    this,
    SLOT(RemoveEmptyItem(QListWidgetItem*))
  );
}

ribi::cmap::QtConceptMapConceptEditDialog::~QtConceptMapConceptEditDialog() noexcept
{
  delete ui;
}

void ribi::cmap::QtConceptMapConceptEditDialog::keyPressEvent(QKeyEvent* e)
{
  if (e->key()  == Qt::Key_Escape) { close(); return; }

  if (
    (e->key() == Qt::Key_Enter || e->key()  == Qt::Key_Return)
    && e->modifiers() & Qt::AltModifier
  )
  {
    on_button_ok_clicked();
    return;
  }

  //QDialog::keyPressEvent(e); //Causes dialog to close unwanted?
}

void ribi::cmap::QtConceptMapConceptEditDialog::on_button_add_clicked()
{
  auto * const new_item = new QTableWidgetItem(
    ui->edit_text->toPlainText()
  );
  const int cur_row_count = ui->list_examples->rowCount();
  assert(ui->list_examples);
  ui->list_examples->insertRow(cur_row_count + 1);
  //ui->list_examples->setRowCount(cur_row_count + 1);
  ui->list_examples->setItem(cur_row_count, 0, new_item);
  ui->edit_text->clear();
  ui->edit_text->setFocus();
}

void ribi::cmap::QtConceptMapConceptEditDialog::RemoveEmptyItem(QListWidgetItem * item)
{
  if (item->text().isEmpty())
  {
    delete item;
    this->update();
  }
}

ribi::cmap::Concept ribi::cmap::QtConceptMapConceptEditDialog::ToConcept() const noexcept
{
  //Name
  const std::string name = ui->edit_concept->toPlainText().toStdString();
  //Examples
  std::vector<Example> v;

  const int n_items = ui->list_examples->rowCount();
  for (int i=0; i != n_items; ++i)
  {
    const auto* const item = ui->list_examples->item(i, 0);
    const Example p(
      item->text().toStdString()
    );
    v.push_back(p);
  }
  assert(n_items == boost::numeric_cast<int>(v.size()));
  return Concept(name, Examples(v));
}

void ribi::cmap::QtConceptMapConceptEditDialog::on_button_ok_clicked()
{
  close();
}
