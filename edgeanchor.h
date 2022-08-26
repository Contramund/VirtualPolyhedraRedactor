#ifndef EDGEANCHOR_H
#define EDGEANCHOR_H


#include "node.h"

#include <QGraphicsView>
#include <QGraphicsItem>



class Node;

class EdgePositionData {
public:
    EdgePositionData();
    EdgePositionData(QPointF a, QPointF b, QPointF c);
    void update(QPointF a, QPointF b, QPointF c);

    QPointF a = {}; // first end of edge
    QPointF b = {}; // second end of edge
    QPointF c = {}; // anchor
    QPointF mid_p = {}; // center of AB
    qreal ab_len = 0; // lenght of AB
    qreal ac_len2 = 0; // square of AC lenght
    qreal mc_len = 0; // lenght of median from C to P(center of AB)
    qreal oc_len = 0; // lenght of OC, where O is center (use uniformity)
    QPointF oc = {}; // vector from C to O
    qreal r = 0; // radius
    QPointF O = {}; // coordinates of point O
    qreal c_right_of = 0; // check if (MC,MA) is a right-pair in terms of orientation where M is a center of AB
    qreal o_right_of = 0; // chect if (MO,MA) is a right-pair in terms of standart orientation on R^2
    qreal arc_angle = 0; // the lenght of arc in degrees
    qreal start_angle = 0;
    qreal oriented_angle = 0;
};

class EdgeAnchor : public QGraphicsEllipseItem {
public:
    EdgeAnchor( Node *n1, Node *n2);
    EdgeAnchor(EdgeAnchor &&ea);
    ~EdgeAnchor();

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    Node *ends[2];
    EdgePositionData position_data;

protected:
    bool isMovingByCursor = false;
    qreal coeff = 0;
};

#endif // EDGEANCHOR_H
