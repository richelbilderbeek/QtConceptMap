#ifndef QTCONCEPTMAPRATEEXAMPLESDIALOG_H
#define QTCONCEPTMAPRATEEXAMPLESDIALOG_H





#include <QDialog>
//#include "qthideandshowdialog.h"
#include "qtconceptmapfwd.h"
#include "conceptmapconcept.h"


namespace Ui { class QtRateExamplesDialog; }

namespace ribi {
namespace cmap {

///Allows the user to rate the examples of a concept
class QtRateExamplesDialog : public QDialog
{
  Q_OBJECT //!OCLINT
  
public:
  explicit QtRateExamplesDialog(
    const Concept& concept,
    QWidget* parent = nullptr
  );
  QtRateExamplesDialog(const QtRateExamplesDialog&) = delete;
  QtRateExamplesDialog& operator=(const QtRateExamplesDialog&) = delete;
  ~QtRateExamplesDialog() noexcept;

  ///Obtain the rated examples
  Examples GetRatedExamples() const;

protected:
  void keyPressEvent(QKeyEvent *);

public slots:
  void on_button_prof_clicked();
  void on_button_organisations_clicked();
  void on_button_social_clicked();
  void on_button_target_audience_clicked();
  void on_button_prof_development_clicked();
  void on_button_misc_clicked();
  void on_button_ti_knowledge_clicked();
  void on_button_ok_clicked();

private:
  Ui::QtRateExamplesDialog *ui;

  ///The concept, which is modified when clicking OK, but remains unmodified when
  ///the user clicks cancel
  Concept m_concept;

  const Concept m_concept_at_start;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPRATEEXAMPLESDIALOG_H
