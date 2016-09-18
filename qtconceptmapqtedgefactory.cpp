#include "qtconceptmapqtedgefactory.h"

#include <cassert>

#include "conceptmapedgefactory.h"
#include "conceptmapedge.h"
#include "conceptmapnode.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmapqtnodefactory.h"



ribi::cmap::QtEdgeFactory::QtEdgeFactory()
{
}

boost::shared_ptr<ribi::cmap::QtEdge> ribi::cmap::QtEdgeFactory::Create(
  const Edge& edge,
  QtNode* const from,
  QtNode* const to
) const noexcept
{
  boost::shared_ptr<QtEdge> qtedge{new QtEdge(edge,from,to)};
  assert(qtedge);
  return qtedge;
}

int ribi::cmap::QtEdgeFactory::GetNumberOfTests() const noexcept
{
  return EdgeFactory().GetNumberOfTests();
}

boost::shared_ptr<ribi::cmap::QtEdge> ribi::cmap::QtEdgeFactory::GetTest(
  const int i,
  const boost::shared_ptr<QtNode>& from,
  const boost::shared_ptr<QtNode>& to
) const noexcept
{
  const auto tests = GetTests(from,to);
  assert(i >= 0);
  assert(i < static_cast<int>(tests.size()));
  return tests[i];
}

std::vector<boost::shared_ptr<ribi::cmap::QtEdge>> ribi::cmap::QtEdgeFactory::GetTests(
  const boost::shared_ptr<QtNode>& from,
  const boost::shared_ptr<QtNode>& to
) const noexcept
{
  std::vector<boost::shared_ptr<QtEdge>> qtedges;
  const auto v = EdgeFactory().GetTests();
  std::transform(v.begin(),v.end(),std::back_inserter(qtedges),
    [from,to](const Edge& c)
    {
      const boost::shared_ptr<QtEdge> q{QtEdgeFactory().Create(c,from.get(),to.get())};
      //const boost::shared_ptr<QtEdge> q{QtEdgeFactory().Create(c,from,to)};
      assert(q);
      q->GetEdge().GetNode().SetX((from->GetCenterX() + to->GetCenterX()) / 2.0);
      q->GetEdge().GetNode().SetY((from->GetCenterY() + to->GetCenterY()) / 2.0);
      return q;
    }
  );
  assert(GetNumberOfTests() == static_cast<int>(qtedges.size()));
  return qtedges;
}
