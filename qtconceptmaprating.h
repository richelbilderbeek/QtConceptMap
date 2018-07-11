#ifndef QTCONCEPTMAPRATING_H
#define QTCONCEPTMAPRATING_H

//
//
//
//
#include "conceptmapgraphtypes.h"
//#include "conceptmap.h"
#include "conceptmapfwd.h"
//

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
