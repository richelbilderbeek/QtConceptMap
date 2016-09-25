#include "qtconceptmaprating_test.h"

#include "qtconceptmaprating.h"

void ribi::cmap::qtconceptmaprating_test::all_tests()
{
  const Rating r;

  //Number of edges, number of examples
  QVERIFY(r.SuggestComplexity(0, 0) == 0);
  QVERIFY(r.SuggestComplexity(1, 0) == 0);
  QVERIFY(r.SuggestComplexity(1, 1) == 1);
  QVERIFY(r.SuggestComplexity(2, 0) == 1);
  QVERIFY(r.SuggestComplexity(1, 2) == 2);
  QVERIFY(r.SuggestComplexity(2, 1) == 2);

  //Number of examples
  QVERIFY(r.SuggestConcreteness(0) == 0);
  QVERIFY(r.SuggestConcreteness(1) == 0);
  QVERIFY(r.SuggestConcreteness(2) == 1);
  QVERIFY(r.SuggestConcreteness(3) == 1);
  QVERIFY(r.SuggestConcreteness(4) == 2);
  QVERIFY(r.SuggestConcreteness(5) == 2);
  QVERIFY(r.SuggestConcreteness(6) == 2);
  QVERIFY(r.SuggestConcreteness(7) == 2);

  //Number of examples
  QVERIFY(r.SuggestSpecificity(0) == 0);
  QVERIFY(r.SuggestSpecificity(1) == 0);
  QVERIFY(r.SuggestSpecificity(2) == 1);
  QVERIFY(r.SuggestSpecificity(3) == 1);
  QVERIFY(r.SuggestSpecificity(4) == 2);
  QVERIFY(r.SuggestSpecificity(5) == 2);
  QVERIFY(r.SuggestSpecificity(6) == 2);
  QVERIFY(r.SuggestSpecificity(7) == 2);
}
