//---------------------------------------------------------------------------
/*
QtConceptMap, Qt classes for display and interaction with ConceptMap
Copyright (C) 2013-2016 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/CppQtConceptMap.htm
//---------------------------------------------------------------------------
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include "qtconceptmaprateconcepttallydialog.h"

#include <cassert>
#include <sstream>
#include <numeric>

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <QKeyEvent>

#include "conceptmapconceptfactory.h"

#include "conceptmapcenternodefactory.h"
#include "conceptmap.h"
#include "conceptmapfactory.h"
#include "conceptmapconcept.h"
#include "conceptmapedge.h"
#include "conceptmapedgefactory.h"
#include "conceptmapnode.h"
#include "conceptmapnodefactory.h"
#include "conceptmapexample.h"
#include "conceptmapedge.h"
#include "conceptmapexamples.h"

#include "qtconceptmaprating.h"
#include "trace.h"
#include "ui_qtconceptmaprateconcepttallydialog.h"
#pragma GCC diagnostic pop

ribi::cmap::QtRateConceptTallyDialog::QtRateConceptTallyDialog(
  const ConceptMap& conceptmap,
  QWidget *parent)
  : QtHideAndShowDialog(parent),
    ui(new Ui::QtRateConceptTallyDialog),
    m_data{CreateData(conceptmap)},
    m_focus_name{GetFocusName(conceptmap)}
{
  ui->setupUi(this);

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
    const Concept concept = std::get<1>(row);
    const int example_index = std::get<2>(row);

    if (example_index == -1)
    {
      ShowNoExample(concept, row_index, row, conceptmap);
    }
    else
    {
      ShowExample(concept, example_index, row_index);
    }
  }

  //Set text on top
  ui->label_concept_name->setText(
    ("Voorbeelden/toelichting bij concept: " + m_focus_name).c_str()
  );

  ui->table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  ui->label_debug->setVisible(true);
  QObject::connect(ui->table,SIGNAL(cellChanged(int,int)),this,SLOT(OnCellChanged(int,int)));

  ShowDebugLabel();
}

ribi::cmap::QtRateConceptTallyDialog::~QtRateConceptTallyDialog() noexcept
{
  delete ui;
}

std::vector<ribi::cmap::QtRateConceptTallyDialog::Row>
  ribi::cmap::QtRateConceptTallyDialog::CreateData(
  const ConceptMap& /* map */
)
{
  std::vector<Row> data;

  #ifdef FIX_ISSUE_10
  assert(map);
  assert(map->GetFocalNode());

  //Add the focal concept its examples (not its name: this cannot be judged)
  {
    const Concept focal_concept = map->GetFocalNode()->GetConcept();
    const int n_examples = boost::numeric_cast<int>(focal_concept.GetExamples().Get().size());
    for (int i=0; i!=n_examples; ++i)
    {
      Edge empty_edge;
      data.push_back(std::make_tuple(empty_edge,focal_concept,i));
    }
  }

  //Collect all relations of the focal node of this sub concept map
  for(const Edge edge: map->GetEdges())
  {
    //But skip the connections to the focal question
    if (IsCenterNode(*edge.GetTo())
      || IsCenterNode(*edge.GetFrom()))
    {
      continue;
    }


    const Concept concept = edge.GetNode().GetConcept();
    data.push_back(std::make_tuple(edge,concept,-1));
    const int n_examples = boost::numeric_cast<int>(concept.GetExamples().Get().size());
    for (int i=0; i!=n_examples; ++i)
    {
      Edge empty_edge;
      data.push_back(std::make_tuple(empty_edge,concept,i));
    }
  }
  #endif // FIX_ISSUE_10
  return data;
}

int ribi::cmap::QtRateConceptTallyDialog::GetSuggestedComplexity() const
{
  //Tally the edges that contribute to complexity
  const int n_edges = std::accumulate(m_data.begin(),m_data.end(),0,
    [](int init, const Row& row)
      {
        return init + (std::get<2>(row) == -1 && std::get<1>(row).GetIsComplex() ? 1 : 0);
      }
    );

  //Tally the examples that contribute to complexity
  const int n_examples = std::accumulate(m_data.begin(),m_data.end(),0,
    [](int init, const Row& row)
      {
        const int index = std::get<2>(row);
        if (index == -1) return init + 0;
        assert(index < static_cast<int>(std::get<1>(row).GetExamples().Get().size()));
        return init + (std::get<1>(row).GetExamples().Get()[index].GetIsComplex() ? 1 : 0);
      }
    );
  const int n_tallied = n_examples + n_edges;
  if (n_tallied < 2) return 0;
  if (n_tallied < 4) return 1;
  return 2;
}

int ribi::cmap::QtRateConceptTallyDialog::GetSuggestedConcreteness() const
{
  //Tally the examples that contribute to concreteness
  const int n_examples = std::accumulate(m_data.begin(),m_data.end(),0,
    [](int init, const Row& row)
      {
        const int index = std::get<2>(row);
        if (index == -1) return init + 0;
        assert(index < static_cast<int>(std::get<1>(row).GetExamples().Get().size()));
        return init + (std::get<1>(row).GetExamples().Get()[index].GetIsConcrete() ? 1 : 0);
      }
    );
  const int n_tallied = n_examples;
  if (n_tallied < 2) return 0;
  if (n_tallied < 4) return 1;
  return 2;
}

int ribi::cmap::QtRateConceptTallyDialog::GetSuggestedSpecificity() const
{
  //Tally the examples that contribute to specificity
  const int n_examples = std::accumulate(m_data.begin(),m_data.end(),0,
    [](int init, const Row& row)
      {
        const int index = std::get<2>(row);
        if (index == -1) return init + 0;
        assert(index < static_cast<int>(std::get<1>(row).GetExamples().Get().size()));
        return init + (std::get<1>(row).GetExamples().Get()[index].GetIsSpecific() ? 1 : 0);
      }
    );
  const int n_tallied = n_examples;
  if (n_tallied < 2) return 0;
  if (n_tallied < 4) return 1;
  return 2;
}

void ribi::cmap::QtRateConceptTallyDialog::keyPressEvent(QKeyEvent * event)
{
  if (event->key() == Qt::Key_Escape) { close(); return; }
  if ( (event->modifiers() & Qt::ControlModifier)
    && (event->modifiers() & Qt::ShiftModifier)
    && event->key() == Qt::Key_T)
  {
    //Translate
    this->setWindowTitle("Relevance of illustrations");
    {
      ui->label_concept_name->setText(
        ("Illustrations and relations of the cluster: " + m_focus_name).c_str()
      );
    }
    {
      QTableWidgetItem * const item = new QTableWidgetItem;
      item->setText("Illustrations and relations of the cluster:");
      ui->table->setHorizontalHeaderItem(3,item);
    }
    {
      const int x = GetSuggestedComplexity();
      const int c = GetSuggestedConcreteness();
      const int s = GetSuggestedSpecificity();
      std::stringstream m;
      m << "Complexity: " << x << ", concreteness: " << c << ", specificity: " << s;
      ui->label_debug->setText(m.str().c_str());
    }
    return;
  }
}

void ribi::cmap::QtRateConceptTallyDialog::OnCellChanged(int row_index, int col)
{
  assert(row_index >= 0 && row_index < static_cast<int>(m_data.size()));
  assert(col >= 0 && col < 4);
  const QTableWidgetItem * const item = ui->table->item(row_index,col);
  assert(item);
  const Row& row = m_data[row_index];
  Concept concept = std::get<1>(row);
  const int index = std::get<2>(row);

  if (index == -1)
  {
    //Concept name
    switch (col)
    {
      case 0: concept.SetIsComplex( item->checkState() == Qt::Checked ); break;
      case 1: break; //Empty cell
      case 2: break; //Empty cell
      case 3: break; //It's read-only! //concept.SetName( item->text().toStdString() ); break;
      default: throw std::logic_error(
        "QtRateConceptTallyDialog::OnCellChanged: unknown col, index -1"
      );
    }
  }
  else
  {
    //Concept example
    assert(index < static_cast<int>(concept.GetExamples().Get().size()));
    Example& example = concept.GetExamples().Get()[index];
    switch (col)
    {
      case 0: example.SetIsComplex( item->checkState() == Qt::Checked ); break;
      case 1: example.SetIsConcrete( item->checkState() == Qt::Checked ); break;
      case 2: example.SetIsSpecific( item->checkState() == Qt::Checked ); break;
      case 3: break; //It's read-only! //example->SetText( item->text().toStdString() ); break;
      default: throw std::logic_error(
        "QtRateConceptTallyDialog::OnCellChanged: unknown col, index not -1"
      );
    }
  }
  ShowDebugLabel();
}

void ribi::cmap::QtRateConceptTallyDialog::on_button_ok_clicked()
{
  close();
}

void ribi::cmap::QtRateConceptTallyDialog::resizeEvent(QResizeEvent *)
{
  const int small_col_width = 28;
  ui->table->setColumnWidth(0, small_col_width);
  ui->table->setColumnWidth(1, small_col_width);
  ui->table->setColumnWidth(2, small_col_width);
  const int extra_space = 8;
  ui->table->setColumnWidth(3,ui->table->width() - (3 * small_col_width) - (3 * extra_space));
}


void ribi::cmap::QtRateConceptTallyDialog::ShowDebugLabel() const noexcept
{
  const int x = GetSuggestedComplexity();
  const int c = GetSuggestedConcreteness();
  const int s = GetSuggestedSpecificity();
  std::stringstream m;
  m << "Complexiteit: " << x << ", concreetheid: " << c << ", specificiteit: " << s;
  ui->label_debug->setText(m.str().c_str());
}


void ribi::cmap::QtRateConceptTallyDialog::ShowExample(
  const Concept& concept,
  const int example_index,
  const int row_index
) const noexcept
{
  assert(example_index < static_cast<int>(concept.GetExamples().Get().size()));
  const int n_cols = 4;

  const Example& example = concept.GetExamples().Get()[example_index];
  //Display index'th example
  for (int col_index=0; col_index!=n_cols; ++col_index)
  {
    if (col_index == 3)
    {
      //Text
      QTableWidgetItem * const item = new QTableWidgetItem;
      item->setFlags(
          Qt::ItemIsSelectable
        | Qt::ItemIsEnabled);
      const std::string s = example.GetText();
      item->setText(s.c_str());
      ui->table->setItem(row_index, col_index, item);
    }
    else
    {
      QTableWidgetItem * const item = new QTableWidgetItem;
      item->setFlags(
        Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable
      );
      switch (col_index)
      {
        case 0: item->setCheckState(example.GetIsComplex()
          ? Qt::Checked : Qt::Unchecked);
        break;
        case 1: item->setCheckState(example.GetIsConcrete()
          ? Qt::Checked : Qt::Unchecked);
        break;
        case 2: item->setCheckState(example.GetIsSpecific()
          ? Qt::Checked : Qt::Unchecked);
        break;
        default:
          throw std::logic_error(
            "ribi::cmap::QtRateConceptTallyDialog::"
            "QtRateConceptTallyDialog: Unknown col index"
          );
      }
      ui->table->setItem(row_index, col_index, item);
    }
  }
}

void ribi::cmap::QtRateConceptTallyDialog::ShowNoExample(
  const Concept& concept,
  const int row_index,
  const Row& row,
  const ConceptMap& conceptmap
) const noexcept
{
  //Display concept text
  //Put X checkbox in the relation's name
  //Keep C and S columns empty
  {
    //Put X checkbox in the relation's name in column[0]
    const int column = 0;
    QTableWidgetItem * const i = new QTableWidgetItem;
    i->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    i->setCheckState(concept.GetIsComplex() ? Qt::Checked : Qt::Unchecked);
    ui->table->setItem(row_index, column, i);
  }
  {
    //Put uneditable nothing column[1]
    const int column = 1;
    QTableWidgetItem * const i = new QTableWidgetItem;
    i->setFlags(Qt::ItemIsEnabled);
    ui->table->setItem(row_index, column, i);
  }
  {
    //Put uneditable nothing column[2]
    const int column = 2;
    QTableWidgetItem * const i = new QTableWidgetItem;
    i->setFlags(Qt::ItemIsEnabled);
    ui->table->setItem(row_index, column, i);
  }
  {
    //Put the relation's name in place
    QTableWidgetItem * const i = new QTableWidgetItem;
    i->setFlags(
        Qt::ItemIsSelectable
      | Qt::ItemIsEnabled
    );
    const EdgeDescriptor edge { std::get<0>(row) };
    const bool center_is_from {
      ribi::cmap::GetFrom(edge,conceptmap) == ribi::cmap::GetFirstNode(conceptmap)
    };
    const Node other {
      center_is_from ? ribi::cmap::GetTo(edge,conceptmap)
      : ribi::cmap::GetFrom(edge, conceptmap)
    };
    const std::string s {
        "via '"
      + concept.GetName() + "' verbonden met '"
      + other.GetConcept().GetName()
      + "'"
    };
    i->setText(s.c_str());

    const int column = 3;
    ui->table->setItem(row_index, column, i);
  }
}
