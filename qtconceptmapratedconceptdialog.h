#ifndef QTCONCEPTMAPRATEDCONCEPTDIALOG_H
#define QTCONCEPTMAPRATEDCONCEPTDIALOG_H

#include <string>
#include <vector>
#include <QDialog>
#include "qtconceptmapfwd.h"
#include "qtconceptmaprole.h"
#include "conceptmapnode.h"
#include "conceptmap.h"
#include "qtconceptmaprole.h"

namespace Ui { class QtConceptMapRatedConceptDialog; }

namespace ribi {
namespace cmap {

class QtConceptMapRatedConceptDialog : public QDialog
{
  Q_OBJECT //!OCLINT
  
public:

  ///In the role of student, no ratings are shown
  explicit QtConceptMapRatedConceptDialog(
    const ConceptMap& conceptmap,
    const Node& node,
    const Role role,
    QWidget *parent = 0
  );
  QtConceptMapRatedConceptDialog(const QtConceptMapRatedConceptDialog&) = delete;
  QtConceptMapRatedConceptDialog& operator=(const QtConceptMapRatedConceptDialog&) = delete;
  ~QtConceptMapRatedConceptDialog() noexcept;

private:
  Ui::QtConceptMapRatedConceptDialog *ui;

  void DisplayEdges(
    const ConceptMap& conceptmap,
    const Node& node,
    const Role role
  ) noexcept;

  void DisplayHeading(
    const Node& node,
    const Role role
  ) noexcept;

  std::string GetFromArrowText(
    const Edge& edge, const ConceptMap& conceptmap
  ) const noexcept;

  std::string GetToArrowText(
    const Edge& edge, const ConceptMap& conceptmap
  ) const noexcept;

  void PutExamplesInList(
    const Node& node,
    const Role role
  ) noexcept;
};

///Convert examples to HTML list items:
///
/// <li>First example</li>
/// <li>Second example</li>
///
///Will produce an empty string if there are no examples
std::string ToHtmlListItems(
  const Examples& examples,
  const Role role
) noexcept;

} //~namespace cmap

} //~namespace ribi

#endif // QTCONCEPTMAPRATEDCONCEPTDIALOG_H
