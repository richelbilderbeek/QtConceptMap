#ifndef QTCONCEPTMAPRATECONCEPTTALLYDIALOGCLOSER_H
#define QTCONCEPTMAPRATECONCEPTTALLYDIALOGCLOSER_H

#include <QDialog>

namespace ribi {
namespace cmap {

///Object to close a popped-up QtRateConceptTallyDialog
class QtRateConceptTallyDialogCloser : public QObject
{
  Q_OBJECT //!OCLINT

  public:
  QtRateConceptTallyDialogCloser();
  virtual ~QtRateConceptTallyDialogCloser();
  public slots:
  void Modify();
  void PressCancel();
  void PressOk();
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPRATECONCEPTTALLYDIALOGCLOSER_H
