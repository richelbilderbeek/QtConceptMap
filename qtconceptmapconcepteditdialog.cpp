#include "qtconceptmapconcepteditdialog.h"

#include <iostream>
#include <cassert>
#include <QScrollBar>
#include <QTableView>
#include "ui_qtconceptmapconcepteditdialog.h"

/*void ribi::cmap::QtConceptMapConceptEditDialog::resize_window_to_examples_widget_size()
{
    int total_height_increase=300;
    QtConceptMapConceptEditDialog::setFixedHeight(800);
    //ui->examples_widget->resizeRowsToContents();
    //ui->edit_text->clear();
    //ui->edit_text->setFocus();
    for(int i=0; i<ui->examples_widget->rowCount(); ++i)
    {
        //QString text = ui->examples_widget->item(i,0)->text();
        //std::string text_string =text.toUtf8().constData();
        int n_characters = text_string.length();
        int n_characters_for_new_line =88; //amount of characters that fit on one line
         //text height +padding height *2
        QStringList lines = text.split( "\n", QString::SkipEmptyParts);
        float height_resize =18;
        if(n_characters<n_characters_for_new_line)
        {
            height_resize=21;
        }
        for(int j=0; j<lines.count()-1; ++j)
        {
            height_resize+=17;
        }
        int counter=0;
        while(n_characters>n_characters_for_new_line)
        {
            counter++;
            n_characters_for_new_line+=92;
            if(counter>1)
            {
                height_resize-=0.8;
            }
            height_resize+=17; //text height
        }
        std::cout<<"resize amount: "<<height_resize<<"\n"<<std::flush;
        total_height_increase +=ui->examples_widget->rowHeight(i);//height_resize
        std::cout<<"Height row "<< i  << " :"<<ui->examples_widget->rowHeight(i) << "\n" << std::flush;
        //ui->examples_widget->setWordWrap(true);

    }
    std::cout<<"total height: "<< total_height_increase <<"\n"<<std::flush;
    ui->examples_widget->setWordWrap(true);
    ui->examples_widget->resizeRowsToContents();
    ui->edit_text->clear();
    ui->edit_text->setFocus();

    int correction=14; //for some reason it doesnt work without the correction

    total_height_increase -=correction;
    QtConceptMapConceptEditDialog::setFixedHeight(total_height_increase);//total_height_increase); //increases the height of the window when a new line is added

    if(QtConceptMapConceptEditDialog::size().height()> 800)
    {
        QtConceptMapConceptEditDialog::setFixedHeight(800);
    }


}*/

ribi::cmap::QtConceptMapConceptEditDialog::QtConceptMapConceptEditDialog(
  const Concept& c,
  const EditType edit_type,
  QWidget* parent)
  : QDialog(parent),
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
  //Add the name
  ui->edit_concept->setPlainText(c.GetName().c_str());
  //Add the examples
  ui->examples_widget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  const int n_examples = CountExamples(c);
  const auto& examples = c.GetExamples().Get();
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

  //If I use the function resize_window_to_examples_widget_size(), when the program starts,
  //all the row sizes will be read as 177 because of the table->resizeRowsToContents.
  //I have no idea why...
  auto table =ui->examples_widget;
  int n_rows=table->verticalHeader()->count();
  int scrollBarHeight=table->horizontalScrollBar()->height();
  int horizontalHeaderHeight=table->horizontalHeader()->height();
  int rowTotalHeight=0;

  for (int i = 0; i < n_rows; ++i)
  {
      rowTotalHeight+=table->rowHeight(i);//verticalHeader()->sectionSize(i);
      //std::cout<<"Height row "<< i  << " :"<<tableView->verticalHeader()->sectionSize(i) << "\n" << std::flush;
  }
  QtConceptMapConceptEditDialog::setFixedHeight(rowTotalHeight+horizontalHeaderHeight+scrollBarHeight + 300);
  table->resizeRowsToContents();
  ui->edit_text->clear();
  ui->edit_text->setFocus();
  //resize_window_to_examples_widget_size();
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

void ribi::cmap::QtConceptMapConceptEditDialog::resize_window_to_examples_widget_size()
{
        auto table =ui->examples_widget;
        int n_rows=table->verticalHeader()->count();
        int scrollBarHeight=table->horizontalScrollBar()->height();
        int horizontalHeaderHeight=table->horizontalHeader()->height();
        int rowTotalHeight=0;
        table->resizeRowsToContents();
        for (int i = 0; i < n_rows; ++i)
        {
            rowTotalHeight+=table->rowHeight(i);//verticalHeader()->sectionSize(i);
            //std::cout<<"Height row "<< i  << " :"<<tableView->verticalHeader()->sectionSize(i) << "\n" << std::flush;
        }
        QtConceptMapConceptEditDialog::setFixedHeight(rowTotalHeight+horizontalHeaderHeight+scrollBarHeight + 260);
        table->resizeRowsToContents();
        ui->edit_text->clear();
        ui->edit_text->setFocus();
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

void ribi::cmap::QtConceptMapConceptEditDialog::on_button_add_clicked()//QTableView *tableView)
{
  auto * const new_item = new QTableWidgetItem(
    ui->edit_text->toPlainText()
  );
  assert(ui->examples_widget);
  const int cur_row_count = ui->examples_widget->rowCount();

  ui->examples_widget->setRowCount(cur_row_count + 1);
  ui->examples_widget->setItem(cur_row_count, 0, new_item);

   resize_window_to_examples_widget_size();
}

void ribi::cmap::QtConceptMapConceptEditDialog
  ::RemoveEmptyItem(QTableWidgetItem * item)
{
  //ui->examples_widget->item->text();
  if (item->text().isEmpty())
  {
    ui->examples_widget->removeRow(item->row());

    resize_window_to_examples_widget_size();
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
