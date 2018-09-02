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
class QtEditConceptDialogTest;
class QtEditConceptDialogCloser;

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

  bool HasUserClickedOk() const noexcept { return m_user_clicked_ok; }
  ///The concept being modified
  Concept ToConcept() const noexcept;

  //void keyPressEvent(QKeyEvent *);

protected:
  void showEvent(QShowEvent *);

private slots:
  void on_button_add_clicked();
  void RemoveEmptyItem(QTableWidgetItem * item);

  void on_button_ok_clicked();
  void on_button_cancel_clicked();

private:
  ribi::cmap::QNumberedColumn * const m_examples;
  Ui::QtConceptMapEditConceptDialog *ui;

  bool m_user_clicked_ok{false};

  friend class QtEditConceptDialogTest;
  friend class QtEditConceptDialogCloser;
};

///Get all the examples' texts
QVector<QString> GetExamplesText(const Concept& concept) noexcept;

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPEDITCONCEPTDIALOG_H
