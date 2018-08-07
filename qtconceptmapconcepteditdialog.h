#ifndef QTCONCEPTMAPCONCEPTEDITDIALOG_H
#define QTCONCEPTMAPCONCEPTEDITDIALOG_H

#include <QDialog>
#include <QTableView>
#include "conceptmapconcept.h"

struct QTableWidgetItem;

namespace Ui { class QtConceptMapConceptEditDialog; }

namespace ribi {
namespace cmap {

struct QNumberedColumn;
struct QtConceptMapConceptEditDialogTest;

class QtConceptMapConceptEditDialog final : public QDialog
{
  Q_OBJECT //!OCLINT
  
public:
  /// concept: a stand-alone node
  /// relation: a node on an edge

  enum EditType { concept, relation };

  ///concept is not const as user might want to modify it
  ///concept is only modified if user clicks OK
  explicit QtConceptMapConceptEditDialog(
    const Concept& concept,
    const EditType edit_type,
    QWidget* parent = nullptr
  );
  void resize_window_to_examples_widget_size();
    void verticalResizeTableViewToContents();
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

private:
  ribi::cmap::QNumberedColumn * const m_examples;

  Ui::QtConceptMapConceptEditDialog *ui;

  friend class QtConceptMapConceptEditDialogTest;
};

///Get all the examples' texts
QVector<QString> GetExamplesText(const Concept& concept) noexcept;

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPCONCEPTEDITDIALOG_H
