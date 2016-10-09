#ifndef qtconceptmaprateexamplesdialog_test_H
#define qtconceptmaprateexamplesdialog_test_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class qtconceptmaprateexamplesdialog_test : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void get_rated_examples();
  void key_presses();
};

} //~namespace cmap
} //~namespace ribi

#endif // qtconceptmaprateexamplesdialog_test_H
