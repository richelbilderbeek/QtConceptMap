#ifndef QTCONCEPTMAPRATING_TEST_H
#define QTCONCEPTMAPRATING_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtConceptMapRatingTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void default_suggestions();
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPRATING_TEST_H
