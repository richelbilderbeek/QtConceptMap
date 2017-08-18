#ifndef QTCONCEPTMAP_COMMANDSETCONCEPT_TEST_H
#define QTCONCEPTMAP_COMMANDSETCONCEPT_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtConceptMapCommandSetConceptTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:

  //To do/review:



  ///Parse from a string
  void Parse() const noexcept;

  ///Parsing a nonsense string should fails
  void ParseNonsenseFails() const noexcept;

  //Done:

};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAP_COMMANDSETCONCEPT_TEST_H

