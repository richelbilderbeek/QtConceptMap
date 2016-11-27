#ifndef QTCONCEPTMAPEXAMPLESDIALOG_H
#define QTCONCEPTMAPEXAMPLESDIALOG_H

#include <memory>
#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "conceptmapexamples.h"
#include "qthideandshowdialog.h"
#include "qtconceptmapfwd.h"
#pragma GCC diagnostic pop

namespace Ui { class QtExamplesDialog; }

namespace ribi {
namespace cmap {

///Displays and modifies Examples
class QtExamplesDialog : public ribi::QtHideAndShowDialog
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

#endif // QTCONCEPTMAPEXAMPLESDIALOG_H
