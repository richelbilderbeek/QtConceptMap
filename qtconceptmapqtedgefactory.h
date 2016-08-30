#ifndef QTCONCEPTMAPQTEDGEFACTORY_H
#define QTCONCEPTMAPQTEDGEFACTORY_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <boost/shared_ptr.hpp>
#include "qthideandshowdialog.h"
#include "qtconceptmapqtedge.h"

#include "qtconceptmapfwd.h"
#pragma GCC diagnostic pop

namespace ribi {
namespace cmap {

struct QtEdgeFactory
{
  QtEdgeFactory();

  boost::shared_ptr<QtEdge> Create(
    const Edge& node,
    QtNode* const from,
    //const boost::shared_ptr<QtNode>& from,
    QtNode* const to
    //const boost::shared_ptr<QtNode>& to
  ) const noexcept;


  ///Obtain a Edge or CenterEdge from an XML std::string
  Edge FromXml(const std::string& s) const noexcept;

  int GetNumberOfTests() const noexcept;

  std::vector<boost::shared_ptr<QtEdge>> GetTests(
    const boost::shared_ptr<QtNode>& from,
    const boost::shared_ptr<QtNode>& to
  ) const noexcept;

  boost::shared_ptr<QtEdge> GetTest(
    const int test,
    const boost::shared_ptr<QtNode>& from,
    const boost::shared_ptr<QtNode>& to
  ) const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPQTEDGEFACTORY_H
