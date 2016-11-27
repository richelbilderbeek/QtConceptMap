#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "qtconceptmaprating.h"

#include <cassert>
#include <boost/numeric/conversion/cast.hpp>

#include "conceptmap.h"
#include "conceptmapnode.h"
#include "conceptmapconcept.h"
#include "conceptmapexamples.h"
#pragma GCC diagnostic pop

int ribi::cmap::Rating::SuggestComplexity(
  const int n_edges,
  const int n_examples
) const noexcept
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
  const int n_edges = boost::num_edges(sub_conceptmap);
  assert(boost::num_vertices(sub_conceptmap) > 0);
  const auto pmap = get(boost::vertex_custom_type, sub_conceptmap);
  const auto node = get(pmap, vd);
  const int n_examples = node.GetConcept().GetExamples().Get().size();
  return SuggestComplexity(n_edges,n_examples);
}

int ribi::cmap::Rating::SuggestConcreteness(const int n_examples) const noexcept
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
  const auto pmap = get(boost::vertex_custom_type, sub_conceptmap);
  const auto node = get(pmap, vd);
  const int n_examples = node.GetConcept().GetExamples().Get().size();
  return SuggestConcreteness(n_examples);
}

int ribi::cmap::Rating::SuggestSpecificity(const int n_examples) const noexcept
{
  return SuggestConcreteness(n_examples);
}

int ribi::cmap::Rating::SuggestSpecificity(
  const ConceptMap& sub_conceptmap,
  const VertexDescriptor& vd
) const noexcept
{
  assert(boost::num_vertices(sub_conceptmap) > 0);
  const auto pmap = get(boost::vertex_custom_type, sub_conceptmap);
  const auto node = get(pmap, vd);
  const int n_examples = node.GetConcept().GetExamples().Get().size();
  return SuggestSpecificity(n_examples);
}
