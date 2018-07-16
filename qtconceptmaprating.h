#ifndef QTCONCEPTMAPRATING_H
#define QTCONCEPTMAPRATING_H

#include "conceptmapgraphtypes.h"
#include "conceptmapfwd.h"

namespace ribi {
namespace cmap {

std::map<std::pair<int, int>, int> CreateDefaultRatingComplexity() noexcept;

///Functions to help with rating
struct Rating
{
  Rating(
    const std::map<std::pair<int, int>, int>& rating_complexity = CreateDefaultRatingComplexity()
  );

  ///Calculate a suggested complexity
  int SuggestComplexity(
    const ConceptMap& sub_conceptmap,
    const VertexDescriptor& vd
  ) const noexcept;

  ///Calculate a suggested concreteness
  int SuggestConcreteness(
    const ConceptMap& sub_conceptmap,
    const VertexDescriptor& vd
  ) const noexcept;

  ///Calculate a suggested specificity
  int SuggestSpecificity(
    const ConceptMap& sub_conceptmap,
    const VertexDescriptor& vd
  ) const noexcept;

  private:
  const std::map<std::pair<int, int>, int> m_rating_complexity;
  std::map<int, int> m_rating_concreteness;
  std::map<int, int> m_rating_specificity;
};

///Calculate a suggested complexity
int SuggestComplexity(const int n_edges, const int n_examples) noexcept;

///Calculate a suggested concreteness
int SuggestConcreteness(const int n_examples) noexcept;

///Calculate a suggested specificity
int SuggestSpecificity(const int n_examples) noexcept;


} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPRATING_H
