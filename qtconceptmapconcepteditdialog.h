#ifndef QTCONCEPTMAPCONCEPTEDITDIALOG_H
#define QTCONCEPTMAPCONCEPTEDITDIALOG_H

#include <QDialog>
#include "conceptmapconcept.h"
struct QTableWidgetItem;

namespace Ui { class QtConceptMapConceptEditDialog; }

namespace ribi {
namespace cmap {

struct QtConceptMapConceptEditDialogTest;

class QtConceptMapConceptEditDialog : public QDialog
{
  Q_OBJECT //!OCLINT
  
public:
  ///concept is not const as user might want to modify it
  ///concept is only modified if user clicks OK
  explicit QtConceptMapConceptEditDialog(
    const Concept& concept,
    QWidget* parent = nullptr
  );
  QtConceptMapConceptEditDialog(const QtConceptMapConceptEditDialog&) = delete;
  QtConceptMapConceptEditDialog& operator=(const QtConceptMapConceptEditDialog&) = delete;
  ~QtConceptMapConceptEditDialog();

  ///The concept being modified
  Concept GetConcept() const noexcept { return ToConcept(); }
  Concept ToConcept() const noexcept;

protected:
  void keyPressEvent(QKeyEvent *);

private slots:
  void on_button_add_clicked();
  void RemoveEmptyItem(QTableWidgetItem * item);
  void showEvent(QShowEvent *);

private:
  Ui::QtConceptMapConceptEditDialog *ui;

  friend class QtConceptMapConceptEditDialogTest;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPCONCEPTEDITDIALOG_H
