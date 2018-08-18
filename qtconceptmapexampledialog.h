#ifndef QTCONCEPTMAPEXAMPLEDIALOG_H
#define QTCONCEPTMAPEXAMPLEDIALOG_H

#ifdef REALLY_NEED_THIS_20180818

#include <QDialog>
#include "conceptmapexample.h"

#include "qtconceptmapfwd.h"


namespace Ui { class QtExampleDialog; }

namespace ribi {
namespace cmap {

///Displays and modifies an Example
class QtExampleDialog final : public QDialog
{
  Q_OBJECT //!OCLINT

public:
  explicit QtExampleDialog(QWidget *parent = 0);
  QtExampleDialog(const QtExampleDialog&) = delete;
  QtExampleDialog& operator=(const QtExampleDialog&) = delete;
  ~QtExampleDialog() noexcept;

  const Example& GetExample() const noexcept { return m_example; }
        Example& GetExample()       noexcept { return m_example; }
  static int GetMinimumHeight(const Example& example) noexcept;
  void SetExample(const Example& example);

private slots:
  void on_box_competency_currentIndexChanged(int index);
  void on_box_is_complex_stateChanged(int arg1);
  void on_box_is_concrete_stateChanged(int arg1);
  void on_box_is_specific_stateChanged(int arg1);
  void on_edit_text_textChanged(const QString &arg1);

private:
  Ui::QtExampleDialog *ui;

  ///The Example to work on
  Example m_example;
};

} //~namespace cmap
} //~namespace ribi

#endif // REALLY_NEED_THIS_20180818

#endif // QTCONCEPTMAPEXAMPLEDIALOG_H
