#include "qtconceptmaprating_test.h"

#include "qtconceptmaprating.h"
#include "conceptmapfactory.h"

void ribi::cmap::QtConceptMapRatingTest::construction()
{
  const Rating r(
    CreateDefaultRatingComplexity(),
    CreateDefaultRatingConcreteness(),
    CreateDefaultRatingSpecificity()
  );

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
    const auto expected = RatingComplexity::SuggestComplexityDefault(n_edges, n_examples);
    QVERIFY(expected == created);
  }
}

void ribi::cmap::QtConceptMapRatingTest::default_suggestions()
{
  //Number of edges, number of examples
  QVERIFY(RatingComplexity::SuggestComplexityDefault(0, 0) == 0);
  QVERIFY(RatingComplexity::SuggestComplexityDefault(0, 1) == 0);
  QVERIFY(RatingComplexity::SuggestComplexityDefault(0, 2) == 1);
  QVERIFY(RatingComplexity::SuggestComplexityDefault(0, 3) == 1);
  QVERIFY(RatingComplexity::SuggestComplexityDefault(0, 4) == 2);
  QVERIFY(RatingComplexity::SuggestComplexityDefault(0, 5) == 2);
  QVERIFY(RatingComplexity::SuggestComplexityDefault(1, 0) == 0);
  QVERIFY(RatingComplexity::SuggestComplexityDefault(1, 1) == 1);
  QVERIFY(RatingComplexity::SuggestComplexityDefault(1, 2) == 1);
  QVERIFY(RatingComplexity::SuggestComplexityDefault(1, 3) == 2);
  QVERIFY(RatingComplexity::SuggestComplexityDefault(1, 4) == 2);
  QVERIFY(RatingComplexity::SuggestComplexityDefault(1, 5) == 2);
  QVERIFY(RatingComplexity::SuggestComplexityDefault(2, 0) == 1);
  QVERIFY(RatingComplexity::SuggestComplexityDefault(2, 1) == 1);
  QVERIFY(RatingComplexity::SuggestComplexityDefault(2, 2) == 2);
  QVERIFY(RatingComplexity::SuggestComplexityDefault(2, 3) == 2);
  QVERIFY(RatingComplexity::SuggestComplexityDefault(2, 4) == 2);
  QVERIFY(RatingComplexity::SuggestComplexityDefault(2, 5) == 2);
  QVERIFY(RatingComplexity::SuggestComplexityDefault(3, 0) == 1);
  QVERIFY(RatingComplexity::SuggestComplexityDefault(3, 1) == 2);
  QVERIFY(RatingComplexity::SuggestComplexityDefault(3, 2) == 2);
  QVERIFY(RatingComplexity::SuggestComplexityDefault(3, 3) == 2);
  QVERIFY(RatingComplexity::SuggestComplexityDefault(3, 4) == 2);
  QVERIFY(RatingComplexity::SuggestComplexityDefault(3, 5) == 2);
  //Number of examples
  QVERIFY(RatingConcreteness::SuggestConcretenessDefault(0) == 0);
  QVERIFY(RatingConcreteness::SuggestConcretenessDefault(1) == 0);
  QVERIFY(RatingConcreteness::SuggestConcretenessDefault(2) == 1);
  QVERIFY(RatingConcreteness::SuggestConcretenessDefault(3) == 1);
  QVERIFY(RatingConcreteness::SuggestConcretenessDefault(4) == 2);
  QVERIFY(RatingConcreteness::SuggestConcretenessDefault(5) == 2);
  QVERIFY(RatingConcreteness::SuggestConcretenessDefault(6) == 2);
  QVERIFY(RatingConcreteness::SuggestConcretenessDefault(7) == 2);

  //Number of examples
  QVERIFY(RatingSpecificity::SuggestSpecificityDefault(0) == 0);
  QVERIFY(RatingSpecificity::SuggestSpecificityDefault(1) == 0);
  QVERIFY(RatingSpecificity::SuggestSpecificityDefault(2) == 1);
  QVERIFY(RatingSpecificity::SuggestSpecificityDefault(3) == 1);
  QVERIFY(RatingSpecificity::SuggestSpecificityDefault(4) == 2);
  QVERIFY(RatingSpecificity::SuggestSpecificityDefault(5) == 2);
  QVERIFY(RatingSpecificity::SuggestSpecificityDefault(6) == 2);
  QVERIFY(RatingSpecificity::SuggestSpecificityDefault(7) == 2);
}
