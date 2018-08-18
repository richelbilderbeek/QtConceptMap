#ifndef QTCONCEPTMAPEXAMPLESDIALOG_H
#define QTCONCEPTMAPEXAMPLESDIALOG_H

#ifdef REALLY_NEED_THIS_20180818

#include <memory>
#include <vector>

#include "conceptmapexamples.h"
#include <QDialog>
#include "qtconceptmapfwd.h"


namespace Ui { class QtExamplesDialog; }

namespace ribi {
namespace cmap {

///Displays and modifies Examples
class QtExamplesDialog final : public QDialog
{
  Q_OBJECT //!OCLINT

public:
  explicit QtExamplesDialog(QWidget *parent = 0);
  QtExamplesDialog(const QtExamplesDialog&) = delete;
  QtExamplesDialog& operator=(const QtExamplesDialog&) = delete;
  ~QtExamplesDialog() noexcept;

  void SetExamples(const Examples& examples);
  const Examples& GetExamples() const noexcept { return m_examples; }
        Examples& GetExamples()       noexcept { return m_examples; }

  static int GetMinimumHeight(const Examples& examples) noexcept;

private:
  Ui::QtExamplesDialog *ui;
  std::vector<std::shared_ptr<QtExampleDialog>> m_dialogs;
  Examples m_examples;
};

} //~namespace ribi
} //~namespace cmap

#endif // REALLY_NEED_THIS_20180818

#endif // QTCONCEPTMAPEXAMPLESDIALOG_H
