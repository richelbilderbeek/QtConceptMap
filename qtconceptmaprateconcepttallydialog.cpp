#include "qtconceptmaprateconcepttallydialog.h"

#include <QDebug>
#include <QKeyEvent>

#include "ui_qtconceptmaprateconcepttallydialog.h"

ribi::cmap::QtRateConceptTallyDialog::QtRateConceptTallyDialog(
  const ConceptMap& conceptmap,
  const ribi::cmap::Rating& rating,
  QWidget *parent
) : QDialog(parent),
    ui(new Ui::QtRateConceptTallyDialog),
    m_data{CreateData(conceptmap)},
    m_rating{rating}
{
  ui->setupUi(this);


  //Set text on top
  ui->label_concept_name->setText(
    QString("Voorbeelden/toelichting bij concept: ")
      + QString(GetFocusName(conceptmap).c_str())
  );

  DisplayData();

  QObject::connect(
    ui->table,
    SIGNAL(cellChanged(int, int)),
    this,
    SLOT(OnCellChanged(int, int))
  );

  UpdateRatingLabel();
}

ribi::cmap::QtRateConceptTallyDialog::~QtRateConceptTallyDialog() noexcept
{
  delete ui;
}

ribi::cmap::QtRateConceptTallyDialog::Data
  ribi::cmap::QtRateConceptTallyDialog::CreateData(
  const ConceptMap& map
)
{
  if (boost::num_vertices(map) == 0)
  {
    throw std::invalid_argument("Cannot create data from empty concept map");
  }

  std::vector<Row> rows;
  //Add the focal concept its examples (not its name: this cannot be tallied)
  {
    const VertexDescriptor vd = *boost::vertices(map).first;
    assert(vd == VertexDescriptor()); //Reminder!
    const Concept focal_concept = map[vd].GetConcept();
    const int n_examples{CountExamples(focal_concept)};
    for (int i=0; i!=n_examples; ++i)
    {
      rows.emplace_back(
        vd,
        EdgeDescriptor(*boost::edges(map).second),
        focal_concept,
        i,
        QString(GetExample(focal_concept, i).GetText().c_str())
      );
    }
  }

  //Collect all relations of the focal node of this sub concept map
  const auto eip = boost::edges(map);
  for (auto ei = eip.first; ei != eip.second; ++ei)
  {
    const EdgeDescriptor ed = *ei;
    assert(ed != EdgeDescriptor());

    //But skip the connections to the focal question
    if (IsCenterNode(GetFrom(ed, map))
      || IsCenterNode(GetTo(ed, map)))
    {
      continue;
    }

    const Edge edge = GetEdge(*ei, map);
    const Concept concept = edge.GetNode().GetConcept();
    const bool center_is_from {
      ribi::cmap::GetFrom(edge, map) == ribi::cmap::GetFirstNode(map)
    };
    const Node other {
      center_is_from ? ribi::cmap::GetTo(edge, map)
      : ribi::cmap::GetFrom(edge, map)
    };
    const QString s {
        QString("via '")
      + concept.GetName().c_str() + "' verbonden met '"
      + other.GetConcept().GetName().c_str()
      + "'"
    };

    rows.emplace_back(
      VertexDescriptor(*boost::vertices(map).second), //Out of range
      ed,
      concept,
      -1,
      s
    );
    const int n_examples = CountExamples(concept);
    for (int i=0; i!=n_examples; ++i)
    {
      rows.emplace_back(
        VertexDescriptor(*boost::vertices(map).second),
        ed,
        concept,
        i,
        QString(GetExample(edge, i).GetText().c_str())
      );
    }
  }
  return rows;
}

void ribi::cmap::QtRateConceptTallyDialog::DisplayData()
{
  ui->table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  const int n_rows = static_cast<int>(m_data.size());
  ui->table->setRowCount(n_rows);
  ui->table->setWordWrap(true);

  //From https://stackoverflow.com/questions/9544122
  //  /how-to-word-wrap-text-in-the-rows-and-columns-of-a-qtablewidget
  connect(
    ui->table->horizontalHeader(),
    SIGNAL(sectionResized(int, int, int)),
    ui->table,
    SLOT(resizeRowsToContents())
  );

  for (int row_index=0; row_index!=n_rows; ++row_index)
  {
    const Row& row = m_data[row_index];
    const int example_index{std::get<3>(row)};
    if (example_index == -1)
    {
      ShowNoExample(row_index, row);
    }
    else
    {
      ShowExample(row_index, row);
    }
  }
}

int ribi::cmap::QtRateConceptTallyDialog::GetNumberOfCheckedComplexExamples() const
{
  const int n_rows{ui->table->rowCount()};
  int n_x_items{0};
  for (int i{0}; i != n_rows; ++i)
  {
    if (
      ui->table->item(i, 1)->flags() & Qt::ItemIsUserCheckable //An example
      && ui->table->item(i, 0)->checkState() == Qt::Checked
    )
    {
      ++n_x_items;
    }
  }
  return n_x_items;
}

int ribi::cmap::QtRateConceptTallyDialog::GetNumberOfCheckedConcreteExamples() const
{
  const int n_rows{ui->table->rowCount()};
  int n_c_examples{0};
  for (int i{0}; i != n_rows; ++i)
  {
    //An example
    if (ui->table->item(i, 1)->flags() & Qt::ItemIsUserCheckable
      && ui->table->item(i, 1)->checkState() == Qt::Checked
    )
    {
      ++n_c_examples;
    }
  }
  return n_c_examples;
}

int ribi::cmap::QtRateConceptTallyDialog::GetNumberOfCheckedSpecificExamples() const
{
  const int n_rows{ui->table->rowCount()};
  int n_s_examples{0};
  for (int i{0}; i != n_rows; ++i)
  {
    //An example
    if (ui->table->item(i, 1)->flags() & Qt::ItemIsUserCheckable
      && ui->table->item(i, 2)->checkState() == Qt::Checked
    ){
      ++n_s_examples;
    }
  }
  return n_s_examples;

}

int ribi::cmap::QtRateConceptTallyDialog::GetNumberOfExamples() const
{
  //The number of checkboxes in the second column
  const int n_rows{ui->table->rowCount()};
  int n_examples{0};
  for (int i{0}; i != n_rows; ++i)
  {
    if (ui->table->item(i, 1)->flags() & Qt::ItemIsUserCheckable)
    {
      //Both second and third column are checkable in an example
      assert(ui->table->item(i, 2)->flags() & Qt::ItemIsUserCheckable);
      ++n_examples;
    }
  }
  return n_examples;
}

int ribi::cmap::QtRateConceptTallyDialog::GetNumberOfComplexRelations() const
{
  //The number of checkboxes in the second column
  const int n_rows{ui->table->rowCount()};
  int n_x_relations{0};
  for (int i{0}; i != n_rows; ++i)
  {
    if (!(ui->table->item(i, 1)->flags() & Qt::ItemIsUserCheckable)
      && ui->table->item(i, 0)->checkState() == Qt::Checked
    ) {
      ++n_x_relations;
    }
  }
  return n_x_relations;
}

int ribi::cmap::QtRateConceptTallyDialog::GetNumberOfRelations() const
{
  //Number of rows that are not an example
  const int n_rows{ui->table->rowCount()};
  const int n_examples{GetNumberOfExamples()};
  return n_rows - n_examples;
}

int ribi::cmap::QtRateConceptTallyDialog::GetSuggestedComplexity() const
{
  return m_rating.SuggestComplexity(
    GetNumberOfComplexRelations(),
    GetNumberOfCheckedComplexExamples()
  );
}

int ribi::cmap::QtRateConceptTallyDialog::GetSuggestedConcreteness() const
{
  return m_rating.SuggestConcreteness(
    GetNumberOfCheckedConcreteExamples()
  );
}

int ribi::cmap::QtRateConceptTallyDialog::GetSuggestedSpecificity() const
{
  return m_rating.SuggestSpecificity(
    GetNumberOfCheckedSpecificExamples()
  );
}

void ribi::cmap::QtRateConceptTallyDialog::keyPressEvent(QKeyEvent * event)
{
  if (event->key() == Qt::Key_Escape) { close(); return; }
}

void ribi::cmap::QtRateConceptTallyDialog::OnCellChanged(
  int /* row_index */, int /* col */ )
{
  UpdateRatingLabel();
}

void ribi::cmap::QtRateConceptTallyDialog::on_button_ok_clicked()
{
  m_has_user_clicked_ok = true;
  close();
}

void ribi::cmap::QtRateConceptTallyDialog::PutNothing(
  const int row, const int col
) noexcept
{
  auto * const i = new QTableWidgetItem;
  i->setFlags(Qt::ItemIsEnabled);
  ui->table->setItem(row, col, i);
}

void ribi::cmap::QtRateConceptTallyDialog::resizeEvent(QResizeEvent *)
{
  const int small_col_width = 28;
  ui->table->setColumnWidth(0, small_col_width);
  ui->table->setColumnWidth(1, small_col_width);
  ui->table->setColumnWidth(2, small_col_width);
  const int extra_space = 8;
  ui->table->setColumnWidth(
    3,
    ui->table->width() - (3 * small_col_width) - (3 * extra_space)
  );
}

void ribi::cmap::QtRateConceptTallyDialog::ShowExample(
  const int row_index,
  const Row& row
) const
{
  auto ToCheckState = [](const bool b) { return b ? Qt::Checked : Qt::Unchecked; };
  const int example_index{std::get<3>(row)};
  assert(example_index >= 0);
  assert(example_index < CountExamples(std::get<2>(row)));
  const Example& example = GetExample(std::get<2>(row), example_index);
  const int n_cols = 4;

  //First three columns, but not the last one
  for (int col_index=0; col_index!=n_cols-1; ++col_index)
  {
    auto * const item = new QTableWidgetItem;
    item->setFlags(
      Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable
    );
    if (col_index == 0)
    {
      item->setCheckState(ToCheckState(example.GetIsComplex()));
    }
    else if (col_index == 1)
    {
      item->setCheckState(ToCheckState(example.GetIsConcrete()));
    }
    else
    {
      assert(col_index == 2);
      item->setCheckState(ToCheckState(example.GetIsSpecific()));
    }
    ui->table->setItem(row_index, col_index, item);
  }
  //Last special column
  {
    const int col_index = 3;
    //Text
    auto * const item = new QTableWidgetItem(std::get<4>(row));
    item->setFlags(
        Qt::ItemIsSelectable
      | Qt::ItemIsEnabled
    );
    ui->table->setItem(row_index, col_index, item);
  }
}

void ribi::cmap::QtRateConceptTallyDialog::ShowNoExample(
  const int row_index,
  const Row& row
) noexcept
{
  // An example index of -1 denotes it is not an example
  assert(std::get<3>(row) == -1);

  const Concept& concept{std::get<2>(row)};

  //Display concept text
  //Put X checkbox in the relation's name
  //Keep C and S columns empty
  {
    //Put X checkbox in the relation's name in column[0]
    const int column = 0;
    auto * const i = new QTableWidgetItem;
    i->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    i->setCheckState(concept.GetIsComplex() ? Qt::Checked : Qt::Unchecked);
    ui->table->setItem(row_index, column, i);
  }
  PutNothing(row_index, 1);
  PutNothing(row_index, 2);
  {
    //Put the relation's name in place
    auto * const i = new QTableWidgetItem;
    i->setFlags(
        Qt::ItemIsSelectable
      | Qt::ItemIsEnabled
    );
    i->setText(std::get<4>(row));

    const int column = 3;
    ui->table->setItem(row_index, column, i);
  }
}

void ribi::cmap::QtRateConceptTallyDialog::UpdateRatingLabel() const noexcept
{
  ui->table->horizontalHeaderItem(0)->setToolTip(
    QString::fromStdString(
      ToHtml(
        m_rating.GetRatingComplexity(),
        GetNumberOfCheckedComplexExamples(),
        GetNumberOfComplexRelations()
      )
    )
  );

  ui->table->horizontalHeaderItem(1)->setToolTip(
    QString::fromStdString(
      ToHtml(
        m_rating.GetRatingConcreteness(),
        GetNumberOfCheckedConcreteExamples()
      )
    )
  );

  ui->table->horizontalHeaderItem(2)->setToolTip(
    QString::fromStdString(
      ToHtml(
        m_rating.GetRatingSpecificity(),
        GetNumberOfCheckedSpecificExamples()
      )
    )
  );

  ui->label_concept_name->setToolTip(
      QString("<ul>\n")
    + QString("  <li><p style='white-space:pre'>Aantal aangevinkte complex voorbeelden: ")
    + QString::number(GetNumberOfCheckedComplexExamples())
    + QString("  </p></li>\n")
    + QString("  <li><p style='white-space:pre'>Aantal aangevinkte concrete voorbeelden: ")
    + QString::number(GetNumberOfCheckedConcreteExamples())
    + QString("  </p></li>\n")
    + QString("  <li><p style='white-space:pre'>Aantal aangevinkte specifieke voorbeelden: ")
    + QString::number(GetNumberOfCheckedSpecificExamples())
    + QString("  </p></li>\n")
    + QString("  <li><p style='white-space:pre'>Totaal aantal voorbeelden: ")
    + QString::number(GetNumberOfExamples())
    + QString("  </p></li>\n")
    + QString("  <li><p style='white-space:pre'>Aantal complex relaties: ")
    + QString::number(GetNumberOfComplexRelations())
    + QString("  </p></li>\n")
    + QString("  <li><p style='white-space:pre'>Aantal relaties: ")
    + QString::number(GetNumberOfRelations())
    + QString("  </p></li>\n")
    + QString("</ul>\n")
  );

  std::stringstream m;
  m << "X: " << GetSuggestedComplexity() << ", "
    << "C: " << GetSuggestedConcreteness() << ", "
    << "S: " << GetSuggestedSpecificity()
  ;
  ui->label_rating->setText(m.str().c_str());

}

void ribi::cmap::QtRateConceptTallyDialog::Write(
  ConceptMap& conceptmap
) const
{
  assert(boost::num_vertices(conceptmap) >= 1);
  const int n_rows = m_data.size();
  assert(ui->table->rowCount() == n_rows);

  for (int row_index = 0; row_index != n_rows; ++row_index)
  {
    const Row& row = m_data[row_index];
    const int example_index{std::get<3>(row)};
    if (std::get<0>(row) == *boost::vertices(conceptmap).second)
    {
      //Edge
      assert(std::get<1>(row) != *boost::edges(conceptmap).second);
      const EdgeDescriptor ed = std::get<1>(row);
      if (example_index >= 0)
      {
        //Examples has XCS
        Example& example = GetExample(conceptmap[ed], example_index);
        example.SetIsComplex(
          ui->table->item(row_index, 0)->checkState() == Qt::Checked
        );
        example.SetIsConcrete(
          ui->table->item(row_index, 1)->checkState() == Qt::Checked
        );
        example.SetIsSpecific(
          ui->table->item(row_index, 2)->checkState() == Qt::Checked
        );
      }
      else
      {
        //Relation name only has X
        Edge& edge = conceptmap[ed];
        SetIsComplex(edge,
          ui->table->item(row_index, 0)->checkState() == Qt::Checked
        );
      }
    } // if edge
    else
    {
      //Node
      const VertexDescriptor vd = std::get<0>(row);
      assert(std::get<1>(row) == *boost::edges(conceptmap).second);
      if (example_index >= 0)
      {
        //Examples has XCS
        Example& example = GetExample(conceptmap[vd], example_index);
        example.SetIsComplex(
          ui->table->item(row_index, 0)->checkState() == Qt::Checked
        );
        example.SetIsConcrete(
          ui->table->item(row_index, 1)->checkState() == Qt::Checked
        );
        example.SetIsSpecific(
          ui->table->item(row_index, 2)->checkState() == Qt::Checked
        );
      }
      else
      {
        //Node's name only has X
        Node& node = conceptmap[vd];
        SetIsComplex(node,
          ui->table->item(row_index, 0)->checkState() == Qt::Checked
        );
      } // if node's name
    } // if node
  } // next row_index
}

void ribi::cmap::QtRateConceptTallyDialog::on_button_cancel_clicked()
{
  close();
}
