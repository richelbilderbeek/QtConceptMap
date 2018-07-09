
/*














*/



#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "qtconceptmapconcepteditdialog.h"

#include <cassert>

#include <QKeyEvent>
#include <QObjectList>
#include <QListWidgetItem>

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
    ui(new Ui::QtConceptMapConceptEditDialog),
    m_concept{concept},
    m_concept_original{concept}
{
  ui->setupUi(this);

  //Convert the concept to its GUI elements
  //Add the name
  ui->edit_concept->setPlainText(concept.GetName().c_str());
  //Add the examples
  const std::vector<Example> v = concept.GetExamples().Get();
  std::for_each(v.begin(),v.end(),
    [this](const Example& example)
    {
      assert(!example.GetText().empty());
      QtConceptMapListWidgetItem * const item
        = new QtConceptMapListWidgetItem(example.GetCompetency());
      item->setText(example.GetText().c_str());
      item->setFlags(
            Qt::ItemIsSelectable
          | Qt::ItemIsEnabled
          | Qt::ItemIsEditable
          | Qt::ItemIsDragEnabled
          | Qt::ItemIsDropEnabled
      );
      ui->list_examples->addItem(item);
    }
  );

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
  {
    QListWidgetItem * const item = new QListWidgetItem;
    item->setText(ui->edit_text->toPlainText());
    item->setFlags(
          Qt::ItemIsSelectable
        | Qt::ItemIsEnabled
        | Qt::ItemIsEditable
        | Qt::ItemIsDragEnabled
        | Qt::ItemIsDropEnabled
    );
    ui->list_examples->addItem(item);
  }
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

void ribi::cmap::QtConceptMapConceptEditDialog::on_button_ok_clicked()
{
  #ifndef CONCEPTMAP_WRITE_TO_CONCEPT
  //Name
  const std::string name = ui->edit_concept->toPlainText().toStdString();
  //Examples
  std::vector<Example> v;

  const int n_items = ui->list_examples->count();
  for (int i=0; i!=n_items; ++i)
  {
    const QListWidgetItem * const item = ui->list_examples->item(i);
    const QtConceptMapListWidgetItem * const braw_item
      = dynamic_cast<const QtConceptMapListWidgetItem *>(item)
    ;
    const Competency competency = braw_item
      ? braw_item->m_competency : Competency::uninitialized;
    Example p(
      item->text().toStdString(),
      competency
    );
    v.push_back(p);
  }
  assert(n_items == boost::numeric_cast<int>(v.size()));
  //Set to concept
  const Examples examples(v);
  m_concept.SetName(name);
  m_concept.SetExamples(examples);
  #endif
  close();
}

#ifdef CONCEPTMAP_WRITE_TO_CONCEPT
const ribi::cmap::Concept ribi::cmap::QtConceptMapConceptEditDialog::WriteToConcept() const
{
  //Name
  const std::string name = ui->edit_concept->text().toStdString();
  //Examples
  std::vector<std::shared_ptr<cmap::Example> > v;

  const int n_items = ui->list_examples->count();
  for (int i=0; i!=n_items; ++i)
  {
    const QListWidgetItem * const item = ui->list_examples->item(i);
    const QtConceptMapListWidgetItem * const braw_item
      = dynamic_cast<const QtConceptMapListWidgetItem *>(item);
    const Competency competency = braw_item
      ? braw_item->m_competency : cmap::Competency::uninitialized;
    std::shared_ptr<Example> p(
      cmap::ExampleFactory().Create(
        item->text().toStdString(),
        competency
      )
    );
    v.push_back(p);
  }
  assert(n_items == boost::numeric_cast<int>(v.size()));
  //Set to concept
  const std::shared_ptr<ribi::cmap::Examples> examples(new cmap::Examples(v));
  assert(examples);
  const Concept concept
    = ribi::cmap::ConceptFactory().Create(
      name,
      examples,
      m_rating_complexity,
      m_rating_concreteness,
      m_rating_specificity);

  assert(concept);
  return concept;
}
#endif
