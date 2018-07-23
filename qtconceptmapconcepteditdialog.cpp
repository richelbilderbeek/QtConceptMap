#include "qtconceptmapconcepteditdialog.h"

#include <cassert>
#include "ui_qtconceptmapconcepteditdialog.h"

ribi::cmap::QtConceptMapConceptEditDialog::QtConceptMapConceptEditDialog(
  const Concept& concept,
  QWidget* parent)
  : QDialog(parent),
    ui(new Ui::QtConceptMapConceptEditDialog)
{
  ui->setupUi(this);

  //Convert the concept to its GUI elements
  //Add the name
  ui->edit_concept->setPlainText(concept.GetName().c_str());
  //Add the examples
  ui->examples_widget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  const int n_examples = CountExamples(concept);
  const auto& examples = concept.GetExamples().Get();
  ui->examples_widget->setWordWrap(true);
  ui->examples_widget->setRowCount(n_examples);
  for (int i = 0; i != n_examples; ++i)
  {
    const auto& example = examples[i];
    QTableWidgetItem * const item = new QTableWidgetItem(example.GetText().c_str());
    ui->examples_widget->setItem(i, 0, item);
  }
  //showEvent will fit content to QTableWidgetItem

  //Connect signals and slots
  connect(
    ui->examples_widget,
    SIGNAL(itemChanged(QTableWidgetItem*)),
    this,
    SLOT(RemoveEmptyItem(QTableWidgetItem*))
  );
  connect(
    ui->button_ok,
    SIGNAL(clicked(bool)),
    this,
    SLOT(close())
  );
  int total_height_increase=300;
  for(int i=0; i<ui->examples_widget->rowCount(); ++i)
  {
      QString text = ui->examples_widget->item(i,0)->text();
      std::string text_string =text.toUtf8().constData();
      int n_characters = text_string.length();
      int n_characters_for_new_line =88; //amount of characters that fit on one line
      int height_resize=19; //text height +padding height *2
      QStringList lines = text.split( "\n", QString::SkipEmptyParts);
      for(int j=0; j<lines.count()-1; ++j)
      {
          height_resize+=15;
      }
      while(n_characters>n_characters_for_new_line)
      {
          n_characters_for_new_line+=88;
          height_resize+=15; //text height
      }
      total_height_increase +=height_resize;
  }

  QtConceptMapConceptEditDialog::setFixedHeight(total_height_increase); //increases the height of the window when a new line is added
  ui->examples_widget->resizeRowsToContents();
  ui->edit_text->clear();
  ui->edit_text->setFocus();
  //connect(ui->examples_widget, SIGNAL(cellChanged(int,int)),
  //  ui->examples_widget, SLOT(resizeRowsToContents())
  //);
  /*
  connect(
    ui->examples_widget->horizontalHeader(),
    SIGNAL(sectionResized(int, int, int)),
    ui->examples_widget,
    SLOT(resizeRowsToContents())
  );
  */
}

ribi::cmap::QtConceptMapConceptEditDialog::~QtConceptMapConceptEditDialog()
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
    close();
    return;
  }

  //QDialog::keyPressEvent(e); //Causes dialog to close unwanted?
}

void ribi::cmap::QtConceptMapConceptEditDialog::on_button_add_clicked()
{
  auto * const new_item = new QTableWidgetItem(
    ui->edit_text->toPlainText()
  );
  assert(ui->examples_widget);
  const int cur_row_count = ui->examples_widget->rowCount();

  ui->examples_widget->setRowCount(cur_row_count + 1);
  ui->examples_widget->setItem(cur_row_count, 0, new_item);

  int total_height_increase=300;
  for(int i=0; i<ui->examples_widget->rowCount(); ++i)
  {
      QString text = ui->examples_widget->item(i,0)->text();
      std::string text_string =text.toUtf8().constData();
      int n_characters = text_string.length();
      int n_characters_for_new_line =88; //amount of characters that fit on one line
      int height_resize=19; //text height +padding height *2
      QStringList lines = text.split( "\n", QString::SkipEmptyParts);
      for(int j=0; j<lines.count()-1; ++j)
      {
          height_resize+=15;
      }
      while(n_characters>n_characters_for_new_line)
      {
          n_characters_for_new_line+=88;
          height_resize+=15; //text height
      }
      total_height_increase +=height_resize;
  }

  QtConceptMapConceptEditDialog::setFixedHeight(total_height_increase); //increases the height of the window when a new line is added
  ui->examples_widget->resizeRowsToContents();
  ui->edit_text->clear();
  ui->edit_text->setFocus();
}

void ribi::cmap::QtConceptMapConceptEditDialog
  ::RemoveEmptyItem(QTableWidgetItem * item)
{
  //ui->examples_widget->item->text();
  if (item->text().isEmpty())
  {
    ui->examples_widget->removeRow(item->row());

    int total_height_increase=300;
    for(int i=0; i<ui->examples_widget->rowCount(); ++i)
    {
        QString text = ui->examples_widget->item(i,0)->text();
        std::string text_string =text.toUtf8().constData();
        int n_characters = text_string.length();
        int n_characters_for_new_line =88; //amount of characters that fit on one line
        int height_resize=19; //text height +padding height *2
        QStringList lines = text.split( "\n", QString::SkipEmptyParts);
        for(int j=0; j<lines.count()-1; ++j)
        {
            height_resize+=15;
        }
        while(n_characters>n_characters_for_new_line)
        {
            n_characters_for_new_line+=88;
            height_resize+=15; //text height
        }
        total_height_increase +=height_resize;
    }

    QtConceptMapConceptEditDialog::setFixedHeight(total_height_increase);
    this->update();
  }
}

void ribi::cmap::QtConceptMapConceptEditDialog::showEvent(QShowEvent *)
{
  ui->examples_widget->resizeRowsToContents();
}

ribi::cmap::Concept ribi::cmap::QtConceptMapConceptEditDialog::ToConcept() const noexcept
{
  //Name
  const std::string name = ui->edit_concept->toPlainText().toStdString();
  //Examples
  std::vector<Example> v;

  const int n_items = ui->examples_widget->rowCount();
  for (int i=0; i != n_items; ++i)
  {
    const auto* const item = ui->examples_widget->item(i, 0);
    const Example p(
      item->text().toStdString()
    );
    v.push_back(p);
  }
  assert(n_items == static_cast<int>(v.size()));
  return Concept(name, Examples(v));
}
