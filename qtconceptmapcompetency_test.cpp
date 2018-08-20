#include "qtconceptmapcompetency_test.h"
#include "qtconceptmapcompetency.h"
#include "conceptmapcompetencies.h"
#include <QDataStream>

void ribi::cmap::CompetencyTest::competency_to_color() const noexcept
{
  //Conversion between QColor and Competency
  const std::vector<Competency> v = Competencies().GetAllCompetencies();
  std::for_each(v.begin(),v.end(),
    [](const Competency& competency)
    {
      const QColor color = QtCompetency().CompetencyToColor(competency);
      QCOMPARE(QtCompetency().ColorToCompetency(color), competency);
    }
  );
}


void ribi::cmap::CompetencyTest::competency_to_icon_assume_this_works() const noexcept
{
  const QIcon image_misc = QtCompetency().CompetencyToIcon(Competency::profession);
  const QIcon image_uninitialized = QtCompetency().CompetencyToIcon(Competency::uninitialized);
  QCOMPARE(image_misc.pixmap(14,14).toImage(), image_misc.pixmap(14,14).toImage());
  QCOMPARE(image_uninitialized.pixmap(14,14).toImage(), image_uninitialized.pixmap(14,14).toImage());
  QVERIFY(image_misc.pixmap(14,14).toImage() != image_uninitialized.pixmap(14,14).toImage());
}


void ribi::cmap::CompetencyTest::competency_to_icon() const noexcept
{
  const std::vector<Competency> v = Competencies().GetAllCompetencies();
  std::for_each(v.begin(),v.end(),
    [](const Competency& competency)
    {
      const QIcon icon = QtCompetency().CompetencyToIcon(competency);
      QVERIFY(!icon.isNull());
      QCOMPARE(QtCompetency().IconToCompetency(icon), competency);
    }
  );
}
