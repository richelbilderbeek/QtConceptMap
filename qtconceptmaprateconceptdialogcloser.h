#ifndef QTCONCEPTMAPRATECONCEPTDIALOGCLOSER_H
#define QTCONCEPTMAPRATECONCEPTDIALOGCLOSER_H

#include <QDialog>

namespace ribi {
namespace cmap {

///Object to close a popped-up QtRateConceptDialog
class QtRateConceptDialogCloser : public QObject
{
  Q_OBJECT //!OCLINT

  public:
  QtRateConceptDialogCloser();
  virtual ~QtRateConceptDialogCloser();
  public slots:
  void PressCancel();
  void PressOk();
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPRATECONCEPTDIALOGCLOSER_H
