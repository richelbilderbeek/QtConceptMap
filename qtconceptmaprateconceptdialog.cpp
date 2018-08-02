#include "qtconceptmaprateconceptdialog.h"

#include <QDesktopWidget>
#include <QKeyEvent>

#include "count_if_bundled_vertex.h"
#include "create_direct_neighbour_bundled_edges_and_vertices_subgraph.h"
#include "find_if_first_bundled_vertex.h"
#include "qtconceptmap.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmaprateconcepttallydialog.h"
#include "ui_qtconceptmaprateconceptdialog.h"

ribi::cmap::QtRateConceptDialog::QtRateConceptDialog(
  const QtConceptMap& q,
  const QtNode& qtnode,
  QWidget* parent
): QDialog(parent),
    ui(new Ui::QtRateConceptDialog),
    m_button_ok_clicked{false},
    m_conceptmap{q.ToConceptMap()},
    m_qtconceptmap{std::make_unique<QtConceptMap>(q.GetRating())}
{
  assert(!IsOnEdge(qtnode));

  const auto equal_id_pred =
    [qtnode_id = qtnode.GetId()](const Node& node)
    {
      return node.GetId() == qtnode_id;
    };

  //Rate concept
  assert(count_if_bundled_vertex(m_conceptmap, equal_id_pred) == 1);
  const auto vd = find_if_first_bundled_vertex(
    m_conceptmap,
    equal_id_pred
  );
  const auto subgraph
    = create_direct_neighbour_bundled_edges_and_vertices_subgraph(
      vd, m_conceptmap
    );
  // Start from usual

  if (!boost::num_vertices(m_conceptmap))
  {
    throw std::invalid_argument("Need at least one concept");
  }

  ui->setupUi(this);
  m_qtconceptmap->SetConceptMap(m_conceptmap);

  assert(m_qtconceptmap);
  assert(ui->conceptmap_layout);

  ui->conceptmap_layout->addWidget(m_qtconceptmap.get());
  ui->box_complexity->setCurrentIndex(
    GetFirstNode(m_conceptmap).GetConcept().GetRatingComplexity()
  );
  ui->box_concreteness->setCurrentIndex(
    GetFirstNode(m_conceptmap).GetConcept().GetRatingConcreteness()
  );
  ui->box_specificity->setCurrentIndex(
    GetFirstNode(m_conceptmap).GetConcept().GetRatingSpecificity()
  );
  ui->box_complexity->setFocus();

  //Set suggestions
  DisplaySuggestions();

  //Center the dialog
  {
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->setGeometry(screen.adjusted(64, 64, -64, -64));
    this->move( screen.center() - this->rect().center() );
  }
}

ribi::cmap::QtRateConceptDialog::~QtRateConceptDialog() noexcept
{
  delete ui;
}

void ribi::cmap::QtRateConceptDialog::DisplaySuggestions() noexcept
{
  if (boost::num_vertices(m_conceptmap) > 0)
  {
    const auto& rating = m_qtconceptmap->GetRating();

    // Without vertices, there is no valid vertex descriptor
    assert(boost::num_vertices(m_conceptmap) > 0);
    const auto vd = *vertices(m_conceptmap).first;
    {
      const QString s = "Formeel uitgangspunt: "
        + QString::number(
          rating.SuggestComplexity(m_conceptmap, vd)
        );
      ui->box_complexity->setToolTip(s);
    }
    {
      const QString s = "Formeel uitgangspunt: "
        + QString::number(
          rating.SuggestConcreteness(m_conceptmap, vd)
        );
      ui->box_concreteness->setToolTip(s);
    }
    {
      const QString s = "Formeel uitgangspunt: "
        + QString::number(
          rating.SuggestSpecificity(m_conceptmap, vd)
        );
      ui->box_specificity->setToolTip(s);
    }
  }
  else
  {
    ui->box_complexity->setToolTip("Formeel uitgangspunt: -");
    ui->box_concreteness->setToolTip("Formeel uitgangspunt: -");
    ui->box_specificity->setToolTip("Formeel uitgangspunt: -");
  }
}

int ribi::cmap::QtRateConceptDialog::GetComplexity() const noexcept
{
  return ui->box_complexity->currentIndex();
}

int ribi::cmap::QtRateConceptDialog::GetConcreteness() const noexcept
{
  return ui->box_concreteness->currentIndex();
}

bool ribi::cmap::QtRateConceptDialog::GetOkClicked() const noexcept {
  return m_button_ok_clicked;
}

int ribi::cmap::QtRateConceptDialog::GetSpecificity() const noexcept
{
  return ui->box_specificity->currentIndex();
}

void ribi::cmap::QtRateConceptDialog::keyPressEvent(QKeyEvent* e)
{
  if (e->key()  == Qt::Key_Escape) { close(); return; }
  //QDialog::keyPressEvent(e);
}

void ribi::cmap::QtRateConceptDialog::on_button_ok_clicked()
{
  //Ratings already set by comboboxes
  m_button_ok_clicked = true;
  close();
}

void ribi::cmap::QtRateConceptDialog::on_button_tally_relevancies_clicked()
{
  QtRateConceptTallyDialog d(
    m_conceptmap,
    m_qtconceptmap->GetRating()
  );
  d.exec(); //Keep this dialog visible, as of 2013-08-30
  if (d.HasUserClickedOk())
  {
    ui->box_complexity->setCurrentIndex(d.GetSuggestedComplexity());
    ui->box_concreteness->setCurrentIndex(d.GetSuggestedConcreteness());
    ui->box_specificity->setCurrentIndex(d.GetSuggestedSpecificity());
    d.Write(m_conceptmap);
  }
}
