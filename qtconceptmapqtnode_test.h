#ifndef qtconceptmapqtnode_test_H
#define qtconceptmapqtnode_test_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class qtconceptmapqtnode_test : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void disable_and_enable();
  void qtnode_is_qtroundededitrectitem();
  void qtnode_is_qtroundededitrectitem_with_equal_x();
  void qtnode_is_qtroundededitrectitem_with_equal_y();
  void to_str();
  void to_stream();
};

} //~namespace cmap
} //~namespace ribi

#endif // qtconceptmapqtnode_test_H
