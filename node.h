#ifndef NODE_H
#define NODE_H

#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>

#include <vector>
#include <map>

#include "properties.h"

class triple_edge_base;

class Node : public QGraphicsEllipseItem {
public:
    void update_intersections();
    Node (const QRectF &rect);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    std::vector<triple_edge_base *> *common_edges;
    std::vector<Node *> *common_nodes;
    std::map<std::pair<int,int>,QPointF> *border_intersection;
};


#endif // NODE_H
