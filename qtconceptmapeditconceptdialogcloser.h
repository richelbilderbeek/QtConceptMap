#ifndef QTCONCEPTMAPEDITCONCEPTDIALOGCLOSER_H
#define QTCONCEPTMAPEDITCONCEPTDIALOGCLOSER_H

#include <QDialog>

namespace ribi {
namespace cmap {

class QtEditConceptDialog;

///Object to close a popped-up QtEditConceptDialog
class QtEditConceptDialogCloser : public QObject
{
  Q_OBJECT //!OCLINT

  public:
  QtEditConceptDialogCloser();
  virtual ~QtEditConceptDialogCloser();
  public slots:
  void Modify();
  void ModifyAndOk();
  void PressCancel();
  void PressOk();
  private:
  ///Returns non-nullptr
  ///Will freeze if pop-up absent
  QtEditConceptDialog * GetDialog() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPEDITCONCEPTDIALOGCLOSER_H
