#ifndef TRIPLE_EDGES_H
#define TRIPLE_EDGES_H

#include "edgeanchor.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>

#include <variant>

  // // // // // // // // // // // // // // // // // // // // // // //
 // // // // // // // TripleEdgeBase // // // // // // // // // // //
// // // // // // // // // // // // // // // // // // // // // // //


class triple_edge_base : public EdgeAnchor
{
public:
    triple_edge_base( Node *n1, Node *n2);
    triple_edge_base(triple_edge_base &&te);
    ~triple_edge_base();

    void update_lines();
    void update_collars();
    bool obstackle_for_collar(int end_num) const;
    bool is_obstackle(int end_num) const;
    void setVisible(int isVisible); // 0 - invisible, 1 - transparance, 2 - visible
    virtual qreal angle_needed(const qreal r);
    virtual void paint_middle(const qreal start_angle, const qreal end_angle);

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    // void contextMenuEvent(QContextMenuEvent *event);

private:
    qreal collar_angles[2];
    qreal line_angle[2];
    bool hit_obstackle = false;

    qreal gap_angle = 0;
    qreal middle_start[2];

    QGraphicsPathItem* bg_line[2]; // background line
    QGraphicsPathItem* bg_line_border[2]; // border lines
    QGraphicsPathItem* central_line[2]; // border lines
    QGraphicsPathItem* collar_bg_line[2];
    QGraphicsPathItem* collar_bg_line_border[2];
    QGraphicsPathItem* collar_central_line[2];
};

  // // // // // // // // // // // // // // // // // // // // // // //
 // // // // // // // TripleEdgeCustoms // // // // // // // // // //
// // // // // // // // // // // // // // // // // // // // // // //

enum EdgeType {
    Base,
    Rot213,
    Rot132
};

class triple_edge : public triple_edge_base {
public:
    triple_edge(Node *n1, Node *n2);
    triple_edge(triple_edge_base &&te);
    ~triple_edge();

    EdgeType type = Base;

    qreal angle_needed(const qreal r) override;
    void paint_middle(const qreal start_angle, const qreal end_angle) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    QGraphicsPathItem *upper_line;
    QGraphicsPathItem *central_line;
    QGraphicsPathItem *bottom_line;
    QGraphicsPathItem *bg_area1;
    QGraphicsPathItem *bg_area2;
private:
    void paint_middle_rot1(qreal mode, const qreal start_angle, const qreal end_angle);
};

  // // // // // // // // // // // // // // // // // // // // // // //
 // // // // // // // TripleEdgeEnum // // // // // // // // // // //
// // // // // // // // // // // // // // // // // // // // // // //



#endif // TRIPLE_EDGES_H
