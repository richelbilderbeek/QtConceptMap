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
#include "qtconceptmapratedconceptdialog.h"

#include <cassert>

#include <boost/lexical_cast.hpp>

#include <QScrollBar>
#include <QTimer>

#include "conceptmapconcept.h"
#include "conceptmap.h"
#include "conceptmapedge.h"
#include "conceptmapexample.h"
#include "conceptmapexamples.h"
#include "conceptmapnode.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include "ui_qtconceptmapratedconceptdialog.h"
#pragma GCC diagnostic pop

ribi::cmap::QtConceptMapRatedConceptDialog::QtConceptMapRatedConceptDialog(
  const ConceptMap& conceptmap,
  const Node& node,
  QWidget *parent)
  : QDialog(parent),
    ui(new Ui::QtConceptMapRatedConceptDialog),
    m_timer(new QTimer)
{
  ui->setupUi(this);

  {
    QFont font = ui->list_cluster_relations->font();
    font.setPointSize(8);
    ui->list_concept_examples->setFont(font);
    ui->list_cluster_relations->setFont(font);
  }

  DisplayHeading(node);
  PutExamplesInList(node);
  DisplayEdges(conceptmap, node);

  QObject::connect(
    m_timer,SIGNAL(timeout()),
    this,SLOT(DoResizeLists())
  );
  m_timer->setInterval(1);
  m_timer->start();
}

ribi::cmap::QtConceptMapRatedConceptDialog::~QtConceptMapRatedConceptDialog()
{
  delete ui;
}

void ribi::cmap::QtConceptMapRatedConceptDialog::AddExamples(const Edge& edge) noexcept
{
  for (const Example& example: edge.GetNode().GetConcept().GetExamples().Get())
  {
    ui->list_cluster_relations->addItem(
      new QListWidgetItem(
        ("  " + example.GetText()).c_str()
      )
    );
  }
}

void ribi::cmap::QtConceptMapRatedConceptDialog::DisplayEdges(
  const ConceptMap& conceptmap,
  const Node& node
) noexcept
{
  for (const Edge& edge: GetEdges(conceptmap))
  {
    if (IsConnectedTo(edge, GetCenterNode(conceptmap), conceptmap)) continue;
    if (IsConnectedTo(edge, node, conceptmap))
    {

      //Dependent on arrow
      if (GetFrom(edge, conceptmap) == node && !IsCenterNode(GetTo(edge, conceptmap)))
      {
        const std::string first_arrow
          = edge.HasTailArrow() ? "<- " : "-- ";
        const std::string second_arrow
          = edge.HasHeadArrow() ? " -> " : " -- ";
        const std::string text
          = first_arrow
          + edge.GetNode().GetConcept().GetName()
          + second_arrow
          + GetTo(edge, conceptmap).GetConcept().GetName();
        ui->list_cluster_relations->addItem(new QListWidgetItem(text.c_str()));
      }
      else if (GetTo(edge, conceptmap) == node)
      {
        assert(GetTo(edge, conceptmap) == node);
        const std::string first_arrow  = edge.HasHeadArrow() ? "<- " : "-- ";
        const std::string second_arrow = edge.HasTailArrow() ? " -> " : " -- ";
        const std::string text
          = first_arrow
          + edge.GetNode().GetConcept().GetName()
          + second_arrow
          + GetFrom(edge, conceptmap).GetConcept().GetName()
        ;
        ui->list_cluster_relations->addItem(new QListWidgetItem(text.c_str()));
      }
      //Indendent on arrow: all examples
      AddExamples(edge);

    }
  }
}

void ribi::cmap::QtConceptMapRatedConceptDialog::DisplayHeading(const Node& node) noexcept
{
  ui->label_name->setText(
    ("Cluster bij concept: " + node.GetConcept().GetName()).c_str()
  );
  ui->label_complexity->setText(
    ("Complexiteit: " + boost::lexical_cast<std::string>(
      node.GetConcept().GetRatingComplexity())
    ).c_str()
  );
  ui->label_concreteness->setText(
    ("Concreetheid: " + boost::lexical_cast<std::string>(
      node.GetConcept().GetRatingConcreteness())
    ).c_str()
  );
  ui->label_specificity->setText(
    ("Specificiteit: " + boost::lexical_cast<std::string>(
      node.GetConcept().GetRatingSpecificity())
    ).c_str()
  );
}

void ribi::cmap::QtConceptMapRatedConceptDialog::DoResizeLists() noexcept
{
  //Set the list displaying the concept its height and widt
  bool done = true;
  for (QListWidget * const w:
    { ui->list_cluster_relations, ui->list_concept_examples } )
  {
    if (w->verticalScrollBar()->isVisible())
    {
      done = false;
      const int h = w->height();
      w->setMaximumHeight(h+1);
      w->setMinimumHeight(h+1);
      assert(w->height() == h + 1);
    }
    //Calculate the height of list_cluster_relations from its sizehint its heights
    //Approach 2
    #ifdef USE_APPROACH_2_20131013
    const int n_items = w->count();
    int height = 0;
    for (int i=0; i!=n_items; ++i)
    {
      height += std::max(w->item(i)->sizeHint().height(),16);
    }
    assert(height >= 0);
    w->setMinimumHeight(height);
    w->setMaximumHeight(height);
    #endif
  }
  if (done) m_timer->stop();
  //Approach 1: set the height manually
  //ui->list_concept_examples->setMinimumHeight(
  //  ui->list_concept_examples->count() * font_in_list_height);
  //ui->list_concept_examples->setMaximumHeight(
  //  ui->list_concept_examples->count() * font_in_list_height);
}

void ribi::cmap::QtConceptMapRatedConceptDialog::HideRating() noexcept
{
  ui->label_complexity->hide();
  ui->label_concreteness->hide();
  ui->label_specificity->hide();
}

void ribi::cmap::QtConceptMapRatedConceptDialog::PutExamplesInList(
  const Node& node
) noexcept
{
  for (const Example& example: node.GetConcept().GetExamples().Get())
  {
    ui->list_concept_examples->addItem(
      new QListWidgetItem(
        (boost::lexical_cast<std::string>(
          static_cast<int>(example.GetCompetency())) + ". " + example.GetText()
        ).c_str()
      )
    );
  }
}
