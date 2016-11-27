#ifndef QTCONCEPTMAPRATING_H
#define QTCONCEPTMAPRATING_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
//#include <boost/shared_ptr.hpp>
#include "conceptmap.h"
#pragma GCC diagnostic pop

namespace ribi {
namespace cmap {

///Functions to help with rating
struct Rating
{
  Rating() {}

  ///Calculate a suggested complexity
  int SuggestComplexity(const int n_edges, const int n_examples) const noexcept;

  ///Calculate a suggested complexity
  int SuggestComplexity(
    const ConceptMap& sub_conceptmap,
    const VertexDescriptor& vd
  ) const noexcept;

  ///Calculate a suggested concreteness
  int SuggestConcreteness(const int n_examples) const noexcept;

  ///Calculate a suggested concreteness
  int SuggestConcreteness(
    const ConceptMap& sub_conceptmap,
    const VertexDescriptor& vd
  ) const noexcept;

  ///Calculate a suggested specificity
  int SuggestSpecificity(const int n_examples) const noexcept;

  ///Calculate a suggested specificity
  int SuggestSpecificity(
    const ConceptMap& sub_conceptmap,
    const VertexDescriptor& vd
  ) const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPRATING_H
