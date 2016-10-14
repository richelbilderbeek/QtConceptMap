#ifndef qtconceptmaprateconcepttallydialog_test_H
#define qtconceptmaprateconcepttallydialog_test_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class qtconceptmaprateconcepttallydialog_test : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void construct_with_empty_conceptmap();
  void construct_with_test_conceptmap();
  void key_presses();
  void measure_ui_from_test_concept_map();

};

} //~namespace cmap
} //~namespace ribi

#endif // qtconceptmaprateconcepttallydialog_test_H
