#ifndef QTCONCEPTMAPCOLLECT_H
#define QTCONCEPTMAPCOLLECT_H

#include <cassert>
#include <QGraphicsScene>
#include "qtconceptmapqtnode.h"
#include "qtconceptmapqtedge.h"

#ifndef TRUST_COLLECT_QTEDGES_20180821
template <class T>
[[deprecated]]
std::vector<T*> Collect(const QGraphicsScene&);
///Collect all QGraphicsItems with class T in an unorderly way
template <class T>
std::vector<T*> Collect(const QGraphicsScene& scene)
{
  //Unsure if this works
  std::vector<T*> v;
  const QList<QGraphicsItem *> items = scene.items();
  std::transform(items.begin(), items.end(), std::back_inserter(v),
    [](QGraphicsItem* const item)
    {
      return qgraphicsitem_cast<T*>(item);
    }
  );
  v.erase(std::remove(v.begin(), v.end(),nullptr), v.end());
  assert(std::count(v.begin(), v.end(), nullptr)==0);
  return v;
}
#endif // TRUST_COLLECT_QTEDGES_20180821

#endif // QTCONCEPTMAPCOLLECT_CPP
