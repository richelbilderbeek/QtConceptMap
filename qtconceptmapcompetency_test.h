#ifndef QTCONCEPTMAP_COMPETENCY_TEST_H
#define QTCONCEPTMAP_COMPETENCY_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtConceptMapCompetencyTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void competency_to_color() const noexcept;
  void competency_to_icon_assume_this_works() const noexcept;
  void competency_to_icon() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAP_COMPETENCY_TEST_H
