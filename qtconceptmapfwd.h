#ifndef QTCONCEPTMAPFWD_H
#define QTCONCEPTMAPFWD_H

#include "conceptmapfwd.h"

struct QGraphicsItem;
struct QGraphicsScene;

namespace ribi {

struct QtQuadBezierArrowItem;
struct QtRoundedRectItemDialog;
struct QtRoundedEditRectItem;
struct QtRoundedEditRectItemDialog;
struct QtKeyboardFriendlyGraphicsView;

namespace cmap {

struct Command;
struct QtConceptDialog;
struct QtConceptMap;
struct QtEdge;
struct QtEdgeDialog;
struct QtExampleDialog;
struct QtExamplesDialog;
struct QtItemHighlighter;
struct QtNewArrow;
struct QtNode;
struct QtNodeDialog;   //QtDialog showing a Node
struct QtQtEdgeDialog; //QtDialog showing a QtEdge
struct QtQtNodeDialog; //QtDialog showing a QtNode
struct QtTool;
struct Rating;
} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPFWD_H
