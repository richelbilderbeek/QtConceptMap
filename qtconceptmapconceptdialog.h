
/*














*/



#ifndef QTCONCEPTMAPCONCEPTDIALOG_H
#define QTCONCEPTMAPCONCEPTDIALOG_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
//#include <boost/shared_ptr.hpp>
#include "qthideandshowdialog.h"

#include "qtconceptmapfwd.h"
#include "conceptmapconcept.h"
#pragma GCC diagnostic pop

namespace Ui { class QtConceptDialog; }

namespace ribi {
namespace cmap {

///Displays and modifies a Concept
class QtConceptDialog : public ribi::QtHideAndShowDialog
{
  Q_OBJECT //!OCLINT

public:
  explicit QtConceptDialog(QWidget *parent = 0);
  QtConceptDialog(const QtConceptDialog&) = delete;
  QtConceptDialog& operator=(const QtConceptDialog&) = delete;
  ~QtConceptDialog() noexcept;

  const Concept& GetConcept() const noexcept { return m_concept; }
        Concept& GetConcept()       noexcept { return m_concept; }
  static int GetMinimumHeight(const Concept& concept) noexcept;
  std::string GetUiName() const noexcept;

  void SetConcept(const Concept& concept) noexcept;
  void SetUiName(const std::string& name) noexcept;


private slots:
  void on_box_is_complex_stateChanged(int arg1) noexcept;
  void on_box_rating_complexity_valueChanged(int arg1) noexcept;
  void on_box_rating_concreteness_valueChanged(int arg1) noexcept;
  void on_box_rating_specificity_valueChanged(int arg1) noexcept;
  void on_edit_name_textChanged(const QString &arg1) noexcept;

private:
  Ui::QtConceptDialog *ui;

  ///The Concept to work on
  Concept m_concept;

  QtExamplesDialog * const m_qtexamplesdialog;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPCONCEPTDIALOG_H
