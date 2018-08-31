#ifndef QTCONCEPTMAPEDITCONCEPTDIALOG_H
#define QTCONCEPTMAPEDITCONCEPTDIALOG_H

#include <QDialog>
#include <QTableView>
#include "conceptmapconcept.h"

class Ruffian;
struct QTableWidgetItem;

namespace Ui { class QtConceptMapEditConceptDialog; }

namespace ribi {

namespace cmap {

struct QNumberedColumn;
struct QtEditDialogTest;

class QtEditConceptDialog final : public QDialog
{
  Q_OBJECT //!OCLINT

public:
  /// concept: a stand-alone node
  /// relation: a node on an edge
  enum EditType { concept, relation };

  explicit QtEditConceptDialog(
    const Concept& concept,
    const EditType edit_type,
    QWidget* parent = nullptr
  );
  QtEditConceptDialog(const QtEditConceptDialog&) = delete;
  QtEditConceptDialog& operator=(const QtEditConceptDialog&) = delete;
  ~QtEditConceptDialog();

  ///The concept being modified
  Concept ToConcept() const noexcept;

  void keyPressEvent(QKeyEvent *);

protected:
  void showEvent(QShowEvent *);

private slots:
  void on_button_add_clicked();
  void RemoveEmptyItem(QTableWidgetItem * item);

private:
  ribi::cmap::QNumberedColumn * const m_examples;
  Ui::QtConceptMapEditConceptDialog *ui;

  friend class QtEditDialogTest;
};

///Get all the examples' texts
QVector<QString> GetExamplesText(const Concept& concept) noexcept;

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPEDITCONCEPTDIALOG_H
