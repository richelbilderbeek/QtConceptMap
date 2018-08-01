#ifndef QTCONCEPTMAPRATING_H
#define QTCONCEPTMAPRATING_H

#include "qtconceptmapfwd.h"
#include "conceptmaprating.h"

namespace ribi {
namespace cmap {

///Functions to help with rating
struct QtRating
{
  ///Use CreateDefaultRating for a default-constructed Rating
  QtRating(const Rating& rating);

  ///Calculate a suggested complexity
  int SuggestComplexity(
    const QtConceptMap& qtconceptmap,
    const QtNode& qtnode
  ) const noexcept;

  ///Calculate a suggested concreteness
  int SuggestConcreteness(
    const QtConceptMap& qtconceptmap,
    const QtNode& qtnode
  ) const noexcept;

  ///Calculate a suggested specificity
  int SuggestSpecificity(
    const QtConceptMap& qtconceptmap,
    const QtNode& qtnode
  ) const noexcept;

  private:
  Rating m_rating;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPRATING_H
