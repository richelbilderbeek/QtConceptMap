#ifndef QTCONCEPTMAPQTNODEFACTORY_H
#define QTCONCEPTMAPQTNODEFACTORY_H





#include <memory>
#include "qthideandshowdialog.h"
#include "qtconceptmapfwd.h"
#include "conceptmapnode.h"


namespace ribi {
namespace cmap {

struct QtNodeFactory
{
  QtNodeFactory();

  std::unique_ptr<QtNode> Create(const Node& node) const noexcept;


  ///Obtain a Node or CenterNode from an XML std::string
  Node FromXml(const std::string& s) const noexcept;

  ///Obtain testing nodes
  int GetNumberOfTests() const noexcept;
  std::vector<std::shared_ptr<QtNode>> GetTests() const noexcept;

  ///Will throw if index is invalid
  std::unique_ptr<QtNode> GetTest(const int i) const;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPQTNODEFACTORY_H
