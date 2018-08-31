#ifndef QTCONCEPTMAPCLASSIFYEXAMPLESDIALOGCLOSER_H
#define QTCONCEPTMAPCLASSIFYEXAMPLESDIALOGCLOSER_H

#include <QDialog>

namespace ribi {
namespace cmap {

///Object to close a popped-up QtClassifyExamplesDialog
class QtClassifyExamplesDialogCloser : public QObject
{
  Q_OBJECT //!OCLINT

  public:
  QtClassifyExamplesDialogCloser();
  virtual ~QtClassifyExamplesDialogCloser();
  public slots:
  void Modify();
  void PressCancel();
  void PressOk();
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPCLASSIFYEXAMPLESDIALOGCLOSER_H
