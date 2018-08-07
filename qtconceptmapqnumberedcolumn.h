#ifndef RIBI_CMAP_QTCONCEPTMAP_QNUMBEREDCOLUMN_H
#define RIBI_CMAP_QTCONCEPTMAP_QNUMBEREDCOLUMN_H

#include <QTableWidget>

namespace ribi {
namespace cmap {

class QNumberedColumn : public QTableWidget
{
  Q_OBJECT

public:
  explicit QNumberedColumn(
    const QVector<QString>& text,
    QWidget *parent = 0
  );
  ~QNumberedColumn();
};

} //~namespace cmap
} //~namespace ribi

#endif // RIBI_CMAP_QTCONCEPTMAP_QNUMBEREDCOLUMN_H
