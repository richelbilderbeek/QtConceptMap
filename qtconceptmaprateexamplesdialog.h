#ifndef QTCONCEPTMAPRATEEXAMPLESDIALOG_H
#define QTCONCEPTMAPRATEEXAMPLESDIALOG_H

#include <QDialog>
#include "qtconceptmapfwd.h"
#include "conceptmapconcept.h"

namespace Ui { class QtRateExamplesDialog; }

namespace ribi {
namespace cmap {

///Allows the user to rate the examples of a concept
class QtRateExamplesDialog final : public QDialog
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

  bool HasClickedOk() const noexcept { return m_clicked_ok; }

protected:
  void keyPressEvent(QKeyEvent *) override;

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

  ///Was the OK button clicked to close?
  bool m_clicked_ok = false;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPRATEEXAMPLESDIALOG_H
