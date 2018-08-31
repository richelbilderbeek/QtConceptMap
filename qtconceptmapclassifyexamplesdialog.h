#ifndef QTCONCEPTMAPCLASSIFYEXAMPLESDIALOG_H
#define QTCONCEPTMAPCLASSIFYEXAMPLESDIALOG_H

#include <QDialog>
#include "qtconceptmapfwd.h"
#include "conceptmapconcept.h"

namespace Ui { class QtClassifyExamplesDialog; }

namespace ribi {
namespace cmap {

class QtClassifyExamplesDialogTest;
class QtClassifyExamplesDialogCloser;

///Allows the user to rate the examples of a concept
class QtClassifyExamplesDialog final : public QDialog
{
  Q_OBJECT //!OCLINT

public:
  explicit QtClassifyExamplesDialog(
    const Concept& concept,
    QWidget* parent = nullptr
  );
  QtClassifyExamplesDialog(const QtClassifyExamplesDialog&) = delete;
  QtClassifyExamplesDialog& operator=(const QtClassifyExamplesDialog&) = delete;
  ~QtClassifyExamplesDialog() noexcept;

  ///Obtain the rated examples
  Examples GetRatedExamples() const;

  bool HasUserClickedOk() const noexcept { return m_clicked_ok; }

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

private slots:
  void on_button_cancel_clicked();

private:
  Ui::QtClassifyExamplesDialog *ui;

  ///Was the OK button clicked to close?
  bool m_clicked_ok = false;

  friend class QtClassifyExamplesDialogTest;
  friend class QtClassifyExamplesDialogCloser;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPCLASSIFYEXAMPLESDIALOG_H
