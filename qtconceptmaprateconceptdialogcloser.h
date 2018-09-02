#ifndef QTCONCEPTMAPRATECONCEPTDIALOGCLOSER_H
#define QTCONCEPTMAPRATECONCEPTDIALOGCLOSER_H

#include <QDialog>

namespace ribi {
namespace cmap {

class QtRateConceptDialog;

///Object to close a popped-up QtRateConceptDialog
class QtRateConceptDialogCloser : public QObject
{
  Q_OBJECT //!OCLINT

  public:
  QtRateConceptDialogCloser();
  virtual ~QtRateConceptDialogCloser();
  public slots:
  void Modify();
  void ModifyAndOk();
  void PressCancel();
  void PressOk();
  private:
  ///Get the dialog to close, will be non-nullptr
  ///Will freeze the application if it will not appear
  QtRateConceptDialog * GetDialog() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPRATECONCEPTDIALOGCLOSER_H
