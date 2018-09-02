#ifndef QTCONCEPTMAPCLASSIFYEXAMPLESDIALOGCLOSER_H
#define QTCONCEPTMAPCLASSIFYEXAMPLESDIALOGCLOSER_H

#include <QDialog>

namespace ribi {
namespace cmap {

class QtClassifyExamplesDialog;

///Object to close a popped-up QtClassifyExamplesDialog
class QtClassifyExamplesDialogCloser : public QObject
{
  Q_OBJECT //!OCLINT

  public:
  QtClassifyExamplesDialogCloser();
  virtual ~QtClassifyExamplesDialogCloser();
  public slots:
  void Modify();
  void ModifyAndOk();
  void PressCancel();
  void PressOk();
  private:
  ///Will return non-nullptr
  ///Will freeze if dialog is absent
  QtClassifyExamplesDialog * GetDialog() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPCLASSIFYEXAMPLESDIALOGCLOSER_H
