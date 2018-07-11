#ifndef QTCONCEPTMAPRATEDCONCEPTDIALOG_H
#define QTCONCEPTMAPRATEDCONCEPTDIALOG_H

#include <string>
#include <vector>





#include <QDialog>
#include "qtconceptmapfwd.h"
#include "conceptmapnode.h"
#include "conceptmap.h"


namespace Ui { class QtConceptMapRatedConceptDialog; }

namespace ribi {
namespace cmap {

class QtConceptMapRatedConceptDialog : public QDialog
{
  Q_OBJECT //!OCLINT
  
public:

  explicit QtConceptMapRatedConceptDialog(
    const ConceptMap& conceptmap,
    const Node& node,
    QWidget *parent = 0
  );
  QtConceptMapRatedConceptDialog(const QtConceptMapRatedConceptDialog&) = delete;
  QtConceptMapRatedConceptDialog& operator=(const QtConceptMapRatedConceptDialog&) = delete;
  ~QtConceptMapRatedConceptDialog() noexcept;

  ///Hides the rating. This is used when a student views his/her
  ///work before an assessor has rated his/her work
  void HideRating() noexcept;

private:
  Ui::QtConceptMapRatedConceptDialog *ui;

  void DisplayEdges(const ConceptMap& conceptmap, const Node& node) noexcept;
  void DisplayHeading(const Node& node) noexcept;

  std::string GetFromArrowText(
    const Edge& edge, const ConceptMap& conceptmap
  ) const noexcept;
  std::string GetToArrowText(
    const Edge& edge, const ConceptMap& conceptmap
  ) const noexcept;

  void PutExamplesInList(const Node& node) noexcept;
};

} //~namespace cmap

} //~namespace ribi

#endif // QTCONCEPTMAPRATEDCONCEPTDIALOG_H
