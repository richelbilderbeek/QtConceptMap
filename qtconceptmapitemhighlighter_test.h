#ifndef QTCONCEPTMAPITEMHIGHLIGHTER_TEST_H
#define QTCONCEPTMAPITEMHIGHLIGHTER_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtItemHighlighterTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void ItemCanBeCenterQtNode() const noexcept;
  void ItemCanBeNormalQtNode() const noexcept;
  void ItemCannotBeQtNodeOnQtEdge() const noexcept;
  void NoItemAtConstruction() const noexcept;
  void PutsPreviousItemBackAtUnrotated() const noexcept;
};

} //~namespace cmap
} //~namespace ribi


#endif // QTCONCEPTMAPITEMHIGHLIGHTER_TEST_H
