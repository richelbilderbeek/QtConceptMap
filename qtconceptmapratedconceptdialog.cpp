#include "qtconceptmapratedconceptdialog.h"

#include <cassert>
#include <sstream>

#include <QScrollBar>
#include <QTimer>

#include "conceptmap.h"
#include "conceptmapconcept.h"
#include "conceptmapedge.h"
#include "conceptmapexample.h"
#include "conceptmapexamples.h"
#include "conceptmapnode.h"
#include "ui_qtconceptmapratedconceptdialog.h"

ribi::cmap::QtConceptMapRatedConceptDialog::QtConceptMapRatedConceptDialog(
  const ConceptMap& conceptmap,
  const Node& node,
  const Role role,
  QWidget *parent)
  : QDialog(parent),
    ui(new Ui::QtConceptMapRatedConceptDialog)
{
  ui->setupUi(this);

  assert(ui->label_concept_examples->wordWrap());
  assert(ui->label_concept_examples->textInteractionFlags()
    & Qt::TextInteractionFlag::TextSelectableByMouse);
  assert(ui->label_concept_examples->textInteractionFlags()
    & Qt::TextInteractionFlag::TextSelectableByKeyboard);


  assert(ui->label_cluster_relations->wordWrap());
  assert(ui->label_cluster_relations->textInteractionFlags()
    & Qt::TextInteractionFlag::TextSelectableByMouse);
  assert(ui->label_cluster_relations->textInteractionFlags()
    & Qt::TextInteractionFlag::TextSelectableByKeyboard);

  DisplayHeading(node, role);
  PutExamplesInList(node, role);
  DisplayEdges(conceptmap, node, role);
}

ribi::cmap::QtConceptMapRatedConceptDialog
  ::~QtConceptMapRatedConceptDialog() noexcept
{
  delete ui;
}

void ribi::cmap::QtConceptMapRatedConceptDialog::DisplayEdges(
  const ConceptMap& conceptmap,
  const Node& node,
  const Role role
) noexcept
{
  std::stringstream s;
  for (const Edge& edge: GetEdges(conceptmap))
  {
    //Do not display edges connected to the center node,
    //as there is no text on these anyways
    if (IsCenterNode(GetFrom(edge, conceptmap))
      || IsCenterNode(GetTo(edge, conceptmap))
      || !IsConnectedTo(edge, node, conceptmap)
    )
    {
      continue;
    }
    s << ToHtmlListItems(edge, conceptmap, node, role);
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

void ribi::cmap::QtConceptMapRatedConceptDialog::DisplayHeading(
  const Node& node,
  const Role role) noexcept
{
  {
    std::stringstream s;
    s << "Cluster bij concept: ";
    if (role == Role::assessor)
    {
      s << "("
        << "X:"
        << (GetRatingComplexity(node) == -1
          ? "?" : std::to_string(GetRatingComplexity(node)))
        << ", "
        << "S:"
        << (GetRatingSpecificity(node) == -1
          ? "?" : std::to_string(GetRatingSpecificity(node)))
        << ", "
        << "C:"
        << (GetRatingConcreteness(node) == -1
          ? "?" : std::to_string(GetRatingConcreteness(node)))
        << ") "
      ;
      ui->label_name->setToolTip(
        "X: complexiteit, S: specificiteit, C: concreetheid"
      );
    }
    s << GetText(node);
    ui->label_name->setText(s.str().c_str());
  }
}

std::string ribi::cmap::GetFromArrowText(
  const Edge& edge, const ConceptMap& conceptmap
) noexcept
{
  const std::string first_arrow  = edge.HasTailArrow()
    ? "<img src=\"qrc:/QtConceptMap/pics/ArrowLeft.png\"/>"
    : "<img src=\"qrc:/QtConceptMap/pics/NoArrow.png\"/> "
  ;
  const std::string second_arrow = edge.HasHeadArrow()
    ? " <img src=\"qrc:/QtConceptMap/pics/ArrowRight.png\"/> "
    : " <img src=\"qrc:/QtConceptMap/pics/NoArrow.png\"/> "
  ;
  return first_arrow
    + GetText(edge)
    + second_arrow
    + GetText(GetTo(edge, conceptmap))
  ;
}

std::string ribi::cmap::GetToArrowText(
  const Edge& edge, const ConceptMap& conceptmap
) noexcept
{
  const std::string first_arrow = edge.HasHeadArrow()
    ? "<img src=\"qrc:/QtConceptMap/pics/ArrowLeft.png\"/> "
    : "<img src=\"qrc:/QtConceptMap/pics/NoArrow.png\"/> "
  ;
  const std::string second_arrow = edge.HasTailArrow()
    ? " <img src=\"qrc:/QtConceptMap/pics/ArrowRight.png\"/> "
    : " <img src=\"qrc:/QtConceptMap/pics/NoArrow.png\"/> "
  ;
  return first_arrow
    + GetText(edge)
    + second_arrow
    + GetText(GetFrom(edge, conceptmap))
  ;
}

void ribi::cmap::QtConceptMapRatedConceptDialog::PutExamplesInList(
  const Node& node,
  const Role role
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
      s << "  <li>";
      if (role == Role::assessor)
      {
        s << "("
          << CompetencyToStrDutchShort(example.GetCompetency())
          << ", "
          << (example.GetIsComplex() ? "X:1" : "X:0")
          << ", "
          << (example.GetIsSpecific() ? "S:1" : "S:0")
          << ", "
          << (example.GetIsConcrete() ? "C:1" : "C:0")
          << ") ";
      }
      s << example.GetText() + "</li>\n";

    }
    s << "</ul>";

  }
  ui->label_concept_examples->setText(s.str().c_str());
}

std::string ribi::cmap::ToHtmlListItemFromTo(
  const Edge& edge,
  const ConceptMap& conceptmap,
  const Node& node,
  const Role role,
  const std::function<std::string(const Edge&, const ConceptMap&)> arrow_text_fun
) {
  assert(
    (
      GetFrom(edge, conceptmap) == node
      && !IsCenterNode(GetTo(edge, conceptmap))
    ) || GetTo(edge, conceptmap) == node
  );
  std::stringstream s;
  s << "  <li>";
  if (role == Role::assessor)
  {
    s << "(" << (GetIsComplex(edge) ? "X:1" : "X:0") << ") ";
  }
  s << arrow_text_fun(edge, conceptmap)
    << "</li>\n"
  ;
  return s.str();
}

std::string ribi::cmap::ToHtmlListItemFrom(
  const Edge& edge,
  const ConceptMap& conceptmap,
  const Node& node,
  const Role role
) {
  assert(GetFrom(edge, conceptmap) == node);
  assert(!IsCenterNode(GetTo(edge, conceptmap)));
  return ToHtmlListItemFromTo(edge, conceptmap, node, role, GetFromArrowText);
}

std::string ribi::cmap::ToHtmlListItemTo(
  const Edge& edge,
  const ConceptMap& conceptmap,
  const Node& node,
  const Role role
) {
  assert(GetTo(edge, conceptmap) == node);
  return ToHtmlListItemFromTo(edge, conceptmap, node, role, GetToArrowText);
}

std::string ribi::cmap::ToHtmlListItems(
  const Edge& edge,
  const ConceptMap& conceptmap,
  const Node& node,
  const Role role) noexcept
{
  //Do not display edges connected to the center node,
  //as there is no text on these anyways
  assert(!IsCenterNode(GetFrom(edge, conceptmap)));
  assert(!IsCenterNode(GetTo(edge, conceptmap)));
  assert(IsConnectedTo(edge, node, conceptmap));

  std::stringstream s;
  //Dependent on arrow
  if (GetFrom(edge, conceptmap) == node
    && !IsCenterNode(GetTo(edge, conceptmap)))
  {
    s << ToHtmlListItemFrom(edge, conceptmap, node, role);
  }
  else if (GetTo(edge, conceptmap) == node)
  {
    s << ToHtmlListItemTo(edge, conceptmap, node, role);
  }
  //Indendent on arrow: all examples
  s << ToHtmlListItems(GetExamples(edge), role);
  return s.str();
}

std::string ribi::cmap::ToHtmlListItems(
  const Examples& examples,
  const Role role) noexcept
{
  std::stringstream s;
  for (const Example& example: examples.Get())
  {

    s << "  <li>";
    if (role == Role::assessor)
    {
      s << "(" + CompetencyToStrDutchShort(example.GetCompetency())
        << " "
        << (example.GetIsComplex() ? "X:1" : "X:0")
        << ", "
        << (example.GetIsSpecific() ? "S:1" : "S:0")
        << ", "
        << (example.GetIsConcrete() ? "C:1" : "C:0")
        << ") "
      ;
    }
    s << example.GetText()
      << "</li>\n"
    ;
  }
  return s.str();
}
