#include "qtconceptmaprating.h"

#include <cassert>
#include <cmath>
//#include <boost/numeric/conversion/cast.hpp>

#include "conceptmap.h"
#include "conceptmapnode.h"
#include "conceptmapconcept.h"
#include "conceptmapexamples.h"
#include "get_my_bundled_vertex.h"

ribi::cmap::Rating::Rating(
  const std::map<std::pair<int, int>, int>& rating_complexity
)
  : m_rating_complexity{rating_complexity},
    m_rating_concreteness{},
    m_rating_specificity{}
{

}

std::map<std::pair<int, int>, int> ribi::cmap::CreateDefaultRatingComplexity() noexcept
{
  //first: number of edges
  //second: number of examples
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
  const int n_examples = CountExamples(get_my_bundled_vertex(vd, sub_conceptmap));
  return ::ribi::cmap::SuggestConcreteness(n_examples);
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
  const int n_examples = CountExamples(get_my_bundled_vertex(vd, sub_conceptmap));
  return ::ribi::cmap::SuggestSpecificity(n_examples);
}
