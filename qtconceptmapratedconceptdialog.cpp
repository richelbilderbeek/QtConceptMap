#include "qtconceptmapratedconceptdialog.h"

#include <cassert>
#include <sstream>

#include <boost/lexical_cast.hpp>

#include <QScrollBar>
#include <QTimer>

#include "conceptmapconcept.h"
#include "conceptmap.h"
#include "conceptmapedge.h"
#include "conceptmapexample.h"
#include "conceptmapexamples.h"
#include "conceptmapnode.h"



#include "ui_qtconceptmapratedconceptdialog.h"


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
  std::stringstream s;
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
        s << "  <li>"
          << "(" << (GetIsComplex(edge) ? "X" : ".")
          << ") "
          << GetFromArrowText(edge, conceptmap)
          << "</li>\n"
        ;
      }
      else if (GetTo(edge, conceptmap) == node)
      {
        s << "  <li>"
          << "(" << (GetIsComplex(edge) ? "X" : ".")
          << ") "
          << GetToArrowText(edge, conceptmap)
          << "</li>\n"
        ;
      }
      //Indendent on arrow: all examples
      for (const Example& example: GetExamples(edge).Get())
      {

        s << "  <li>"
          << "(" + CompetencyToStrDutchShort(example.GetCompetency())
          << " "
          << (example.GetIsComplex() ? "X" : "")
          << (example.GetIsSpecific() ? "S" : "")
          << (example.GetIsConcrete() ? "C" : "")
          << ") "
          << example.GetText()
          << "</li>\n";
      }
    }
  }

  std::stringstream text;
  text << "<b>Relaties bij het cluster: </b>";
  if (s.str().empty())
  {
    text << "geen";
  }
  else
  {
    text << "\n<ul>\n" << s.str() << "</ul>";
  }

  ui->label_cluster_relations->setText(text.str().c_str());
}

void ribi::cmap::QtConceptMapRatedConceptDialog::DisplayHeading(const Node& node) noexcept
{
  {
    std::stringstream s;
    s << "Cluster bij concept: "
        << "("
        << "X:"
        << (GetRatingComplexity(node) == -1 ? "?" : std::to_string(GetRatingComplexity(node)))
        << ", "
        << "S:"
        << (GetRatingSpecificity(node) == -1 ? "?" : std::to_string(GetRatingSpecificity(node)))
        << ", "
        << "C:"
        << (GetRatingConcreteness(node) == -1 ? "?" : std::to_string(GetRatingConcreteness(node)))
        << ") "
        << GetText(node)
      ;
    ui->label_name->setText(s.str().c_str());
  }
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
  const std::string first_arrow  = edge.HasTailArrow() ? "&larr; " : "&mdash; ";
  const std::string second_arrow = edge.HasHeadArrow() ? " &rarr; " : " &mdash; ";
  return first_arrow
    + GetText(edge)
    + second_arrow
    + GetText(GetTo(edge, conceptmap))
  ;
}

std::string ribi::cmap::QtConceptMapRatedConceptDialog::GetToArrowText(
  const Edge& edge, const ConceptMap& conceptmap
) const noexcept
{
  const std::string first_arrow  = edge.HasHeadArrow() ? "&larr; " : "&mdash; ";
  const std::string second_arrow = edge.HasTailArrow() ? " &rarr; " : " &mdash; ";
  return first_arrow
    + GetText(edge)
    + second_arrow
    + GetText(GetFrom(edge, conceptmap))
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
  std::stringstream s;
  s << "<b>Voorbeelden/toelichtingen bij concept: </b>";
  if (CountExamples(node) == 0)
  {
    s << "geen";
  }
  else
  {
    s << "\n<ul>\n"
    ;
    for (const Example& example: node.GetConcept().GetExamples().Get())
    {
      s << "  <li>"
        << "(" + CompetencyToStrDutchShort(example.GetCompetency())
        << " "
        << (example.GetIsComplex() ? "X" : "")
        << (example.GetIsSpecific() ? "S" : "")
        << (example.GetIsConcrete() ? "C" : "")
        << ") "
        << example.GetText() + "</li>\n";
      ;
    }
    s << "</ul>";

  }
  ui->label_concept_examples->setText(s.str().c_str());
}
