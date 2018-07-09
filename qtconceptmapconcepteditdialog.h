#ifndef QTCONCEPTMAPCONCEPTEDITDIALOG_H
#define QTCONCEPTMAPCONCEPTEDITDIALOG_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "qthideandshowdialog.h"
#include "qtconceptmapfwd.h"
#include "conceptmapconcept.h"
#pragma GCC diagnostic pop

struct QListWidgetItem;

namespace Ui { class QtConceptMapConceptEditDialog; }

namespace ribi {
namespace cmap {

struct QtConceptMapConceptEditDialogTest;

class QtConceptMapConceptEditDialog : public ribi::QtHideAndShowDialog
{
  Q_OBJECT //!OCLINT
  
public:
  ///concept is not const as user might want to modify it
  ///concept is only modified if user clicks OK
  explicit QtConceptMapConceptEditDialog(
    const Concept& concept,
    QWidget* parent = 0
  );
  QtConceptMapConceptEditDialog(const QtConceptMapConceptEditDialog&) = delete;
  QtConceptMapConceptEditDialog& operator=(const QtConceptMapConceptEditDialog&) = delete;
  ~QtConceptMapConceptEditDialog() noexcept;

  ///The concept being modified
  Concept GetConcept() const noexcept { return ToConcept(); }
  Concept ToConcept() const noexcept;

  const Ui::QtConceptMapConceptEditDialog * GetUi() const noexcept { return ui; }
        Ui::QtConceptMapConceptEditDialog * GetUi()       noexcept { return ui; }

public slots:

  void on_button_add_clicked();
  void on_button_ok_clicked();

protected:
  void keyPressEvent(QKeyEvent *);

private slots:
  void RemoveEmptyItem(QListWidgetItem * item);


private:
  Ui::QtConceptMapConceptEditDialog *ui;

  friend class QtConceptMapConceptEditDialogTest;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPCONCEPTEDITDIALOG_H
