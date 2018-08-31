#ifndef QTCONCEPTMAPEDITCONCEPTDIALOGCLOSER_H
#define QTCONCEPTMAPEDITCONCEPTDIALOGCLOSER_H

#include <QDialog>

namespace ribi {
namespace cmap {

///Object to close a popped-up QtEditConceptDialog
class QtEditConceptDialogCloser : public QObject
{
  Q_OBJECT //!OCLINT

  public:
  QtEditConceptDialogCloser();
  virtual ~QtEditConceptDialogCloser();
  public slots:
  void PressCancel();
  void PressOk();
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPEDITCONCEPTDIALOGCLOSER_H
