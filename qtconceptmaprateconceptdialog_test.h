#ifndef QTCONCEPTMAPRATECONCEPTDIALOG_TEST_H
#define QTCONCEPTMAPRATECONCEPTDIALOG_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtConceptMapRateConceptDialogTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void show_normal_concept_map();
  void show_concept_map_with_only_center_node();
  void show_empty_concept_map();
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPRATECONCEPTDIALOG_TEST_H
