#ifndef QTCONCEPTMAPRATECONCEPTTALLYDIALOGCLOSER_H
#define QTCONCEPTMAPRATECONCEPTTALLYDIALOGCLOSER_H

#include <QDialog>

namespace ribi {
namespace cmap {

class QtRateConceptTallyDialogCloser : public QObject
{
  Q_OBJECT

  public:
  QtRateConceptTallyDialogCloser(QPushButton * const button);
  virtual ~QtRateConceptTallyDialogCloser();
  public slots:
  void Close();
  private:
  QPushButton * const m_button;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPRATECONCEPTTALLYDIALOGCLOSER_H
