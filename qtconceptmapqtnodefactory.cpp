#include "qtconceptmapqtnodefactory.h"

#include <cassert>

#include "conceptmapnodefactory.h"
#include "qtconceptmapqtnode.h"




ribi::cmap::QtNodeFactory::QtNodeFactory()
{

}

std::unique_ptr<ribi::cmap::QtNode> ribi::cmap::QtNodeFactory::Create(
  const Node& node
) const noexcept
{
  std::unique_ptr<QtNode> qtnode{new QtNode(node)};
  assert(qtnode);
  return qtnode;
}

int ribi::cmap::QtNodeFactory::GetNumberOfTests() const noexcept
{
  return static_cast<int>(GetTests().size());
}

std::unique_ptr<ribi::cmap::QtNode> ribi::cmap::QtNodeFactory::GetTest(
  const int i
) const
{
  return Create(NodeFactory().GetTest(i));
}

std::vector<std::shared_ptr<ribi::cmap::QtNode>>
ribi::cmap::QtNodeFactory::GetTests() const noexcept
{
  std::vector<std::shared_ptr<QtNode>> qtnodes;
  const auto v = NodeFactory().GetTests();
  std::transform(v.begin(),v.end(),std::back_inserter(qtnodes),
    [](const Node& c)
    {
      const std::shared_ptr<QtNode> q{QtNodeFactory().Create(c)};
      assert(q);
      return q;
    }
  );
  return qtnodes;
}
