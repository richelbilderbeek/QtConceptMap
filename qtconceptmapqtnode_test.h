#ifndef qtconceptmapqtnode_test_H
#define qtconceptmapqtnode_test_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtConceptMapQtNodeTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void center_qtnode_is_center_node();
  void normal_qtnode_is_not_center_node();

  //Done
  void disable_and_enable();
  void press_nonsense_is_rejected();
  void qtnode_is_qtroundededitrectitem();
  void qtnode_is_qtroundededitrectitem_with_equal_x();
  void qtnode_is_qtroundededitrectitem_with_equal_y();
  void to_str();
  void to_stream();
};

} //~namespace cmap
} //~namespace ribi

#endif // qtconceptmapqtnode_test_H
