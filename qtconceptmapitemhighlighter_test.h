#ifndef QTCONCEPTMAPITEMHIGHLIGHTER_TEST_H
#define QTCONCEPTMAPITEMHIGHLIGHTER_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtItemHighlighterTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void AllTests();
  void Construction();
};

} //~namespace cmap
} //~namespace ribi


#endif // QTCONCEPTMAPITEMHIGHLIGHTER_TEST_H
