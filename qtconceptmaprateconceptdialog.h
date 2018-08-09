#ifndef QTCONCEPTMAPRATECONCEPTDIALOG_H
#define QTCONCEPTMAPRATECONCEPTDIALOG_H

#include <QDialog>

#include "qtconceptmapfwd.h"
#include "conceptmapconcept.h"
#include "conceptmap.h"
#include "qtconceptmaprating.h"

namespace Ui { class QtRateConceptDialog; }
namespace ribi {
namespace cmap {

///Rate the focal concept of a sub-ConceptMap.
class QtRateConceptDialog final : public QDialog
{
  Q_OBJECT //!OCLINT
    
  public:

  explicit QtRateConceptDialog(
    const QtConceptMap& q,
    const QtNode& qtnode,
    QWidget* parent = nullptr
  );

  QtRateConceptDialog(const QtRateConceptDialog&) = delete;
  QtRateConceptDialog& operator=(const QtRateConceptDialog&) = delete;
  ~QtRateConceptDialog() noexcept;

  ///To distinguish between closing the dialog by clicking OK, or by ALT-F4
  bool GetOkClicked() const noexcept;

  int GetComplexity() const noexcept;
  int GetConcreteness() const noexcept;
  int GetSpecificity() const noexcept;

  void Write(QtConceptMap& q, QtNode& qtnode) const;

protected:
  void keyPressEvent(QKeyEvent *);

private slots:
  void on_button_ok_clicked();
  void on_button_tally_relevancies_clicked();


private:
  Ui::QtRateConceptDialog *ui;

  ///To distinguish between closing the dialog by clicking OK, or by ALT-F4
  bool m_button_ok_clicked{false};

  ///The center concept, may be changed when the user clicks OK
  Concept m_concept;

  ///Can be modified by tally
  ConceptMap m_sub_conceptmap;

  const std::unique_ptr<QtConceptMap> m_qtconceptmap;

  ///Display the Rating as tooltips
  void DisplayAsToolTips(const Rating& rating);

  void DisplaySuggestions() noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPRATECONCEPTDIALOG_H
