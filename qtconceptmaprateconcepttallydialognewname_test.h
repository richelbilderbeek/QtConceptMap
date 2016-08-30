#ifndef qtconceptmaprateconcepttallydialognewname_test_H
#define qtconceptmaprateconcepttallydialognewname_test_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class qtconceptmaprateconcepttallydialognewname_test : public QObject
{
    Q_OBJECT

private slots:
  void construct_with_empty_conceptmap();
  void construct_with_test_conceptmap();
  void measure_ui_from_test_concept_map();

};

} //~namespace cmap
} //~namespace ribi

#endif // qtconceptmaprateconcepttallydialognewname_test_H
