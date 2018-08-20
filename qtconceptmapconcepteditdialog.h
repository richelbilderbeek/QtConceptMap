#ifndef QTCONCEPTMAPCONCEPTEDITDIALOG_H
#define QTCONCEPTMAPCONCEPTEDITDIALOG_H

#include <QDialog>
#include <QTableView>
#include "conceptmapconcept.h"

class Ruffian;
struct QTableWidgetItem;

namespace Ui { class QtConceptMapConceptEditDialog; }

namespace ribi {

namespace cmap {

struct QNumberedColumn;
struct QtEditDialogTest;

class QtConceptMapConceptEditDialog final : public QDialog
{
  Q_OBJECT //!OCLINT
  
public:
  /// concept: a stand-alone node
  /// relation: a node on an edge
  enum EditType { concept, relation };

  explicit QtConceptMapConceptEditDialog(
    const Concept& concept,
    const EditType edit_type,
    QWidget* parent = nullptr
  );
  QtConceptMapConceptEditDialog(const QtConceptMapConceptEditDialog&) = delete;
  QtConceptMapConceptEditDialog& operator=(const QtConceptMapConceptEditDialog&) = delete;
  ~QtConceptMapConceptEditDialog();

  ///The concept being modified
  [[deprecated]]
  Concept GetConcept() const noexcept { return ToConcept(); }
  Concept ToConcept() const noexcept;

  void keyPressEvent(QKeyEvent *);

protected:
  void showEvent(QShowEvent *);

private slots:
  void on_button_add_clicked();
  void RemoveEmptyItem(QTableWidgetItem * item);

private:
  ribi::cmap::QNumberedColumn * const m_examples;
  Ui::QtConceptMapConceptEditDialog *ui;

  friend class QtEditDialogTest;
};

///Get all the examples' texts
QVector<QString> GetExamplesText(const Concept& concept) noexcept;

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPCONCEPTEDITDIALOG_H
