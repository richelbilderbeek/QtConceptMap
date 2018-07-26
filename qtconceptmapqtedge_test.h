#ifndef qtconceptmapqtedge_test_H
#define qtconceptmapqtedge_test_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtConceptMapQtEdgeTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:

  //To do/review:

  //Done:
  void all_tests();
  void change_focus();
  void node_must_have_dotted_black_line_when_selected(); //#127
  void node_must_have_white_border_when_not_selected(); //#127
  void show_bounding_rect();
  void to_str();
  void to_stream();
};

} //~namespace cmap
} //~namespace ribi

#endif // qtconceptmapqtedge_test_H
