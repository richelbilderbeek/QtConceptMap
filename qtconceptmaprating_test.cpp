#include "qtconceptmaprating_test.h"

#include "qtconceptmaprating.h"
#include "conceptmapfactory.h"

void ribi::cmap::QtConceptMapRatingTest::construction()
{
  const Rating r;

  const auto sub_conceptmap = ConceptMapFactory().Get11();
  const auto vip = boost::vertices(sub_conceptmap);
  const auto v_end = vip.second;
  for (auto vi = vip.first; vi != v_end; ++vi)
  {
    const auto vd = *vi;
    const auto created = r.SuggestComplexity(sub_conceptmap, vd);
    const int n_edges = boost::num_edges(sub_conceptmap);
    assert(boost::num_vertices(sub_conceptmap) > 0);
    const int n_examples = CountExamples(sub_conceptmap[vd]);
    const auto expected = SuggestComplexity(n_edges, n_examples);
    QVERIFY(expected == created);
  }
}

void ribi::cmap::QtConceptMapRatingTest::default_suggestions()
{
  //Number of edges, number of examples
  QVERIFY(SuggestComplexity(0, 0) == 0);
  QVERIFY(SuggestComplexity(1, 0) == 0);
  QVERIFY(SuggestComplexity(1, 1) == 1);
  QVERIFY(SuggestComplexity(1, 2) == 1);
  QVERIFY(SuggestComplexity(1, 3) == 1);
  QVERIFY(SuggestComplexity(1, 4) == 1);
  QVERIFY(SuggestComplexity(1, 5) == 1);
  QVERIFY(SuggestComplexity(2, 0) == 1);
  QVERIFY(SuggestComplexity(2, 1) == 2);
  QVERIFY(SuggestComplexity(2, 2) == 2);
  QVERIFY(SuggestComplexity(2, 3) == 2);
  QVERIFY(SuggestComplexity(2, 4) == 2);
  QVERIFY(SuggestComplexity(2, 5) == 2);

  //Number of examples
  QVERIFY(SuggestConcreteness(0) == 0);
  QVERIFY(SuggestConcreteness(1) == 0);
  QVERIFY(SuggestConcreteness(2) == 1);
  QVERIFY(SuggestConcreteness(3) == 1);
  QVERIFY(SuggestConcreteness(4) == 2);
  QVERIFY(SuggestConcreteness(5) == 2);
  QVERIFY(SuggestConcreteness(6) == 2);
  QVERIFY(SuggestConcreteness(7) == 2);

  //Number of examples
  QVERIFY(SuggestSpecificity(0) == 0);
  QVERIFY(SuggestSpecificity(1) == 0);
  QVERIFY(SuggestSpecificity(2) == 1);
  QVERIFY(SuggestSpecificity(3) == 1);
  QVERIFY(SuggestSpecificity(4) == 2);
  QVERIFY(SuggestSpecificity(5) == 2);
  QVERIFY(SuggestSpecificity(6) == 2);
  QVERIFY(SuggestSpecificity(7) == 2);
}
