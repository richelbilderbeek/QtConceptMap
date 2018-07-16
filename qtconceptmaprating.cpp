#include "qtconceptmaprating.h"

#include <cassert>
#include <cmath>

#include "conceptmap.h"
#include "conceptmapnode.h"
#include "conceptmapconcept.h"
#include "conceptmapexamples.h"
#include "get_my_bundled_vertex.h"

ribi::cmap::Rating::Rating(
  const std::map<std::pair<int, int>, int>& rating_complexity,
  const std::map<int, int>& rating_concreteness,
  const std::map<int, int>& rating_specificity
)
  : m_rating_complexity{rating_complexity},
    m_rating_concreteness{rating_concreteness},
    m_rating_specificity{rating_specificity}
{
  // [0, 3] number of nodes, [0, 4] number of examples
  assert(m_rating_complexity.size() == 20);
  // [0, 4] examples
  assert(m_rating_concreteness.size() == 5);
  // [0, 4] examples
  assert(m_rating_specificity.size() == 5);
}

ribi::cmap::Rating ribi::cmap::CreateDefaultRating() noexcept
{
  return Rating(
    CreateDefaultRatingComplexity(),
    CreateDefaultRatingConcreteness(),
    CreateDefaultRatingSpecificity()
  );
}

std::map<std::pair<int, int>, int> ribi::cmap::CreateDefaultRatingComplexity() noexcept
{
  // { {number of edges, number of examples}, score }
  return
  {
    { {0, 0}, 0 },
    { {0, 1}, 0 },
    { {0, 2}, 0 },
    { {0, 3}, 0 },
    { {0, 4}, 0 },
    { {1, 0}, 0 },
    { {1, 1}, 1 },
    { {1, 2}, 1 },
    { {1, 3}, 1 },
    { {1, 4}, 1 },
    { {2, 0}, 1 },
    { {2, 1}, 2 },
    { {2, 2}, 2 },
    { {2, 3}, 2 },
    { {2, 4}, 2 },
    { {3, 0}, 2 },
    { {3, 1}, 2 },
    { {3, 2}, 2 },
    { {3, 3}, 2 },
    { {3, 4}, 2 }
  };
}

std::map<int, int> ribi::cmap::CreateDefaultRatingConcreteness() noexcept
{
  // { n_examples, score }
  return
  {
    {0, 0},
    {1, 0},
    {2, 1},
    {3, 1},
    {4, 2}
  };
}

std::map<int, int> ribi::cmap::CreateDefaultRatingSpecificity() noexcept
{
  // { n_examples, score }
  return
  {
    {0, 0},
    {1, 0},
    {2, 1},
    {3, 1},
    {4, 2}
  };
}
int ribi::cmap::SuggestComplexity(
  const int n_edges,
  const int n_examples
) noexcept
{
  return n_edges == 0  || (n_edges == 1 && n_examples == 0)
    ? 0
    : (n_edges == 1 && n_examples > 0) || (n_edges == 2 && n_examples == 0)
      ? 1
      : 2
  ;
}

int ribi::cmap::Rating::SuggestComplexity(
  const ConceptMap& sub_conceptmap,
  const VertexDescriptor& vd
) const noexcept
{
  const int n_edges = std::min(3, static_cast<int>(boost::num_edges(sub_conceptmap)));
  assert(boost::num_vertices(sub_conceptmap) > 0);
  const int n_examples = std::min(4, CountExamples(sub_conceptmap[vd]));
  const auto iter = m_rating_complexity.find( { n_edges, n_examples} );
  assert(iter != std::end(m_rating_complexity));
  assert(iter->second == ::ribi::cmap::SuggestComplexity(n_edges, n_examples));
  return iter->second;
}

int ribi::cmap::SuggestConcreteness(const int n_examples) noexcept
{
  return n_examples < 2
    ? 0
    : n_examples > 1 && n_examples < 4
      ? 1
      : 2
  ;
}

int ribi::cmap::Rating::SuggestConcreteness(
  const ConceptMap& sub_conceptmap,
  const VertexDescriptor& vd
) const noexcept
{
  assert(boost::num_vertices(sub_conceptmap) > 0);
  const int n_examples = CountExamples(sub_conceptmap[vd]);

  const auto iter = m_rating_concreteness.find(n_examples);
  assert(iter != std::end(m_rating_concreteness));
  assert(iter->second == ::ribi::cmap::SuggestConcreteness(n_examples));
  return iter->second;
}

int ribi::cmap::SuggestSpecificity(const int n_examples) noexcept
{
  return SuggestConcreteness(n_examples);
}

int ribi::cmap::Rating::SuggestSpecificity(
  const ConceptMap& sub_conceptmap,
  const VertexDescriptor& vd
) const noexcept
{
  assert(boost::num_vertices(sub_conceptmap) > 0);
  const int n_examples = CountExamples(sub_conceptmap[vd]);
  const auto iter = m_rating_specificity.find(n_examples);
  assert(iter != std::end(m_rating_specificity));
  assert(iter->second == ::ribi::cmap::SuggestSpecificity(n_examples));
  return iter->second;
}
