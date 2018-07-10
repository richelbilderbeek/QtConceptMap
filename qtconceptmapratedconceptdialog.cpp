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
    ui(new Ui::QtConceptMapRatedConceptDialog)
{
  ui->setupUi(this);

  assert(ui->label_concept_examples->wordWrap());
  assert(ui->label_concept_examples->textInteractionFlags() & Qt::TextInteractionFlag::TextSelectableByMouse);
  assert(ui->label_concept_examples->textInteractionFlags() & Qt::TextInteractionFlag::TextSelectableByKeyboard);

  assert(ui->label_cluster_relations->wordWrap());
  assert(ui->label_cluster_relations->textInteractionFlags() & Qt::TextInteractionFlag::TextSelectableByMouse);
  assert(ui->label_cluster_relations->textInteractionFlags() & Qt::TextInteractionFlag::TextSelectableByKeyboard);

  DisplayHeading(node);
  PutExamplesInList(node);
  DisplayEdges(conceptmap, node);
}

ribi::cmap::QtConceptMapRatedConceptDialog::~QtConceptMapRatedConceptDialog()
{
  delete ui;
}

void ribi::cmap::QtConceptMapRatedConceptDialog::DisplayEdges(
  const ConceptMap& conceptmap,
  const Node& node
) noexcept
{
  std::string s = "<b>Relaties bij het cluster:</b>\n"
    "<ul>\n"
  ;
  for (const Edge& edge: GetEdges(conceptmap))
  {
    //Do not display edges connected to the center node,
    //as there is no text on these anyways
    if (IsCenterNode(GetFrom(edge, conceptmap))
      || IsCenterNode(GetTo(edge, conceptmap))
    )
    {
      continue;
    }
    if (IsConnectedTo(edge, node, conceptmap))
    {
      //Dependent on arrow
      if (GetFrom(edge, conceptmap) == node && !IsCenterNode(GetTo(edge, conceptmap)))
      {
        s += "  <li>" + GetFromArrowText(edge, conceptmap) + "</li>\n";
      }
      else if (GetTo(edge, conceptmap) == node)
      {
        s += "  <li>" + GetToArrowText(edge, conceptmap) + "</li>\n";
      }
      //Indendent on arrow: all examples
      const auto examples_texts = CollectExamplesTexts(edge);
      for (const auto& example_text: examples_texts)
      {
        s += "  <li>" + example_text + "</li>\n";
      }
    }
  }
  s += "</ul>";
  ui->label_cluster_relations->setText(s.c_str());
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

std::string ribi::cmap::QtConceptMapRatedConceptDialog::GetFromArrowText(
  const Edge& edge, const ConceptMap& conceptmap
) const noexcept
{
  const std::string first_arrow
    = edge.HasTailArrow() ? "<- " : "-- ";
  const std::string second_arrow
    = edge.HasHeadArrow() ? " -> " : " -- ";
  return first_arrow
    + edge.GetNode().GetConcept().GetName()
    + second_arrow
    + GetTo(edge, conceptmap).GetConcept().GetName()
  ;
}

std::string ribi::cmap::QtConceptMapRatedConceptDialog::GetToArrowText(
  const Edge& edge, const ConceptMap& conceptmap
) const noexcept
{
  const std::string first_arrow  = edge.HasHeadArrow() ? "<- " : "-- ";
  const std::string second_arrow = edge.HasTailArrow() ? " -> " : " -- ";
  return first_arrow
    + edge.GetNode().GetConcept().GetName()
    + second_arrow
    + GetFrom(edge, conceptmap).GetConcept().GetName()
  ;
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
  std::string s =
    "<b>Voorbeelden/toelichtingen bij concept:</b>\n"
    "<ul>\n"
  ;
  for (const Example& example: node.GetConcept().GetExamples().Get())
  {
    s += "  <li>(" + CompetencyToStrDutch(example.GetCompetency())
      + ") " + example.GetText() + "</li>\n";
    ;
  }
  s += "</ul>";
  ui->label_concept_examples->setText(s.c_str());
}
