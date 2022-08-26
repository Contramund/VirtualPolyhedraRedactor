#include "triple_edges.h"

#include "node.h"
#include "edgeanchor.h"
#include "geometric_functions.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QMenu>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QtMath>

#include <math.h>

  // // // // // // // // // // // // // // // // // // // // // // //
 // // // // // // // TripleEdgeBase // // // // // // // // // // //
// // // // // // // // // // // // // // // // // // // // // // //


void triple_edge_base::setVisible(int isVisible) {
    // 0 - invisible, 1 - transparance, 2 - visible

    switch(isVisible) {
        case 2:
            // set collars visible
            collar_bg_line[0]->setVisible(true);
            collar_bg_line[1]->setVisible(true);
            collar_bg_line_border[0]->setVisible(true);
            collar_bg_line_border[1]->setVisible(true);
            collar_central_line[0]->setVisible(true);
            collar_central_line[1]->setVisible(true);

            // set edge visible
            bg_line[0]->setVisible(true);
            bg_line[1]->setVisible(true);
            bg_line_border[0]->setVisible(true);
            bg_line_border[1]->setVisible(true);
            central_line[0]->setVisible(true);
            central_line[1]->setVisible(true);
            setZValue(1);

            // set edge normal color
            {
                QPen green_edge_pen = QPen(QPen(QColor(0,200,100)));
                green_edge_pen.setWidth(_NODE_SIZE-_LINE_WIDTH);
                green_edge_pen.setCapStyle(Qt::FlatCap);
                bg_line[0]->setPen(green_edge_pen);
                bg_line[1]->setPen(green_edge_pen);
            }
            return;

        case 1:
            // set collars invisible
            collar_bg_line[0]->setVisible(false);
            collar_bg_line_border[0]->setVisible(false);
            collar_central_line[0]->setVisible(false);
            collar_bg_line[1]->setVisible(false);
            collar_bg_line_border[1]->setVisible(false);
            collar_central_line[1]->setVisible(false);

            // set edge visible
            bg_line[0]->setVisible(true);
            bg_line[1]->setVisible(true);
            bg_line_border[0]->setVisible(true);
            bg_line_border[1]->setVisible(true);
            central_line[0]->setVisible(true);
            central_line[1]->setVisible(true);
            setZValue(-1);

            // set edge transparent color
            {
                QPen grey_edge_pen = QPen(QPen(QColor(200,200,200)));
                grey_edge_pen.setWidth(_NODE_SIZE-_LINE_WIDTH);
                grey_edge_pen.setCapStyle(Qt::FlatCap);
                bg_line[0]->setPen(grey_edge_pen);
                bg_line[1]->setPen(grey_edge_pen);
            }
            return;

        case 0:
            // set collars invisible
            collar_bg_line[0]->setVisible(false);
            collar_bg_line_border[0]->setVisible(false);
            collar_central_line[0]->setVisible(false);
            collar_bg_line[1]->setVisible(false);
            collar_bg_line_border[1]->setVisible(false);
            collar_central_line[1]->setVisible(false);

            // set edge invisible
            bg_line[0]->setVisible(false);
            bg_line[1]->setVisible(false);
            bg_line_border[0]->setVisible(false);
            bg_line_border[1]->setVisible(false);
            central_line[0]->setVisible(false);
            central_line[1]->setVisible(false);
            return;

        default:
            // do nothing
            return;
    }
}

qreal triple_edge_base::angle_needed(qreal r)
{
    return 30;
}

void triple_edge_base::paint_middle(qreal start_angle, qreal end_angle)
{
    return;
}

void triple_edge_base::update_lines() {
    gap_angle = angle_needed(position_data.r);

    if( position_data.ab_len/position_data.mc_len < 30 ) { // to avoid sharp jerks of ellipse due to the accuracy
        setVisible(hit_obstackle ? 1 : 2);

        for( int end_num = 0; end_num < 2; end_num++) {
            line_angle[end_num] = (std::abs(position_data.oriented_angle) - gap_angle)/2;
            line_angle[end_num] = (position_data.c_right_of < 0 ? 1 : -1) * (1 - end_num * 2) * line_angle[end_num];

            QPainterPath path;
            path.arcMoveTo(
                QRectF(
                    position_data.oc.x()-position_data.r,
                    position_data.oc.y()-position_data.r,
                    2*position_data.r,
                    2*position_data.r
                ),
                position_data.start_angle + (end_num == 0 ? 0 : position_data.oriented_angle)
            );

            path.arcTo(
                QRectF(
                    position_data.oc.x()-position_data.r,
                    position_data.oc.y()-position_data.r,
                    2*position_data.r,
                    2*position_data.r
                ),
                position_data.start_angle + (end_num == 0 ? 0 : position_data.oriented_angle)
                ,
                line_angle[end_num]
            );

            bg_line[end_num]->setPath(path);
            bg_line_border[end_num]->setPath(path);
            central_line[end_num]->setPath(path);

            paint_middle(position_data.start_angle + line_angle[0], position_data.start_angle + position_data.oriented_angle + line_angle[1]);
        }
    } else {
        setVisible(0);
    }
}

bool triple_edge_base::obstackle_for_collar(int end_num) const {
    for( auto item: collar_bg_line_border[end_num]->collidingItems() ) {
        Node* n_item = dynamic_cast<Node*>(item);
        if ( n_item != nullptr && n_item != ends[end_num] ) {
            return true;
        } else {
            if( item->parentItem() != this && item != collar_bg_line[end_num] && item != collar_central_line[end_num] ) {
                triple_edge_base* tr_item = dynamic_cast<triple_edge_base*>(item->parentItem());
                if( tr_item != nullptr && tr_item->ends[end_num] != ends[end_num] && tr_item->ends[1-end_num] != ends[end_num]) {
                    if( boldArcsIntersects( position_data.O, position_data.r,
                            angle_to( ends[end_num]->pos()-position_data.O, QPointF(1.0, 0.0) ),
                            (position_data.c_right_of < 0 ? 1 : -1) * (1 - end_num * 2) * collar_angles[end_num],
                            tr_item->position_data.O, tr_item->position_data.r,
                            tr_item->position_data.start_angle, tr_item->position_data.oriented_angle))
                    {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}


bool triple_edge_base::is_obstackle(int line_end_num) const {
    for( auto item: bg_line_border[line_end_num]->collidingItems() ) {
        triple_edge_base* tr_item = dynamic_cast<triple_edge_base*>(item->parentItem());
        if( tr_item != nullptr && tr_item != this ) {
            for( int end_num = 0; end_num < 2; end_num ++ ){
                if( ends[0] != tr_item->ends[end_num] && ends[1] != tr_item->ends[end_num] && boldArcsIntersects(
                            position_data.O,
                            position_data.r,
                            position_data.start_angle,
                            position_data.oriented_angle,
                            tr_item->position_data.O,
                            tr_item->position_data.r,
                            angle_to( tr_item->ends[end_num]->pos()-tr_item->position_data.O, QPointF(1.0, 0.0) ),
                            (tr_item->position_data.c_right_of < 0 ? 1 : -1) * (1 - end_num * 2) * tr_item->collar_angles[end_num]
                            ))
                {
                    return true;
                }
            }
        }
    }

    return false;
}


void triple_edge_base::update_collars()
{
    hit_obstackle = false;

    for(int node_num = 0; node_num < 2; node_num++) {
        // Find collar angle
        int in_node_index = std::distance(ends[node_num]->common_edges->begin(), std::find(ends[node_num]->common_edges->begin(), ends[node_num]->common_edges->end(), this));

        // Loop have two sequential numbers in node's list, right ??
        if( ends[0] == ends[1] && node_num == 1) {
            in_node_index += 1;
        }

        collar_angles[node_num] = std::acos( 1 - (_NODE_SIZE+_LINE_WIDTH)*(_NODE_SIZE+_LINE_WIDTH)/(8*position_data.r*position_data.r)) * 180.0 / 3.1415926;
        for( int i = 0; i < ends[node_num]->common_edges->size(); i++) {

            if( ends[node_num]->border_intersection->count(std::make_pair(std::min(in_node_index, i), std::max(in_node_index, i))) <= 0 ) {
                continue;
            }
            QPointF some_end = ends[node_num]->border_intersection->at( std::make_pair(std::min(in_node_index, i), std::max(in_node_index, i)) );

            if( some_end != some_end ) { // capture NaN
                hit_obstackle = true;
            }

            qreal sign = line_angle[node_num] > 0 ? 1.0 : -1.0;
            collar_angles[node_num] = std::max(collar_angles[node_num], -sign * (angle_to(ends[node_num]->pos() - position_data.O, some_end - position_data.O)));
        }

        // Draw coresponding path
        QPainterPath path;
        path.arcMoveTo(
            QRectF(
                position_data.O.x()-position_data.r,
                position_data.O.y()-position_data.r,
                2*position_data.r,
                2*position_data.r
            ),
            angle_to(
                ends[node_num]->pos()-position_data.O,
                QPointF(1.0, 0.0)
            )
        );

        path.arcTo(
            QRectF(
                position_data.O.x()-position_data.r,
                position_data.O.y()-position_data.r,
                2*position_data.r,
                2*position_data.r
            ),
            angle_to(
                ends[node_num]->pos()-position_data.O,
                QPointF(1.0, 0.0)
            ),
            (position_data.c_right_of < 0 ? 1 : -1) *
                (1 - node_num * 2) *
                collar_angles[node_num]
        );

        collar_bg_line[node_num]->setPath(path);
        collar_bg_line_border[node_num]->setPath(path);
        collar_central_line[node_num]->setPath(path);
    }

    // Check for obstackles for collars
    if( obstackle_for_collar(0) || obstackle_for_collar(1) ) {
        hit_obstackle = true;
    }

    // Check for obstackles for whole edge
    if( is_obstackle(0) || is_obstackle(1) ) {
        hit_obstackle = true;
    }

    if( hit_obstackle ) {
        setVisible(1);
    } else {
        setVisible(2);
    }
}


triple_edge_base::triple_edge_base(Node *n1, Node *n2) : EdgeAnchor(n1, n2) {
    // INIT PENS
    QPen edge_pen = QPen(QPen(QColor(0,200,100)));
    edge_pen.setWidth(_NODE_SIZE-_LINE_WIDTH);
    edge_pen.setCapStyle(Qt::FlatCap);

    QPen col_edge_pen = QPen(QPen(QColor(250,0,0)));
    col_edge_pen.setWidth(_NODE_SIZE-_LINE_WIDTH);
    col_edge_pen.setCapStyle(Qt::FlatCap);

    QPen border_pen = QPen(QPen(QColor(0,0,0)));
    border_pen.setWidth(_NODE_SIZE + _LINE_WIDTH);
    border_pen.setCapStyle(Qt::FlatCap);

    QPen col_border_pen = QPen(QPen(QColor(0,0,0)));
    col_border_pen.setWidth(_NODE_SIZE + _LINE_WIDTH);
    col_border_pen.setCapStyle(Qt::FlatCap);

    QPen center_pen = QPen(QPen(QColor(0,0,0)));
    center_pen.setWidth(_LINE_WIDTH);
    center_pen.setCapStyle(Qt::FlatCap);

    QPen col_center_pen = QPen(QPen(QColor(0,0,0)));
    col_center_pen.setWidth(_LINE_WIDTH);
    col_center_pen.setCapStyle(Qt::FlatCap);

    // INIT LINES :

    for( int end_num = 0; end_num < 2; end_num ++) {
        // * background line
        this->bg_line[end_num] = new QGraphicsPathItem(this);
        //bg_line[end_num]->setFlag(QGraphicsItem::ItemStacksBehindParent);
        bg_line[end_num]->setPen(edge_pen);

        // * border lines
        this->bg_line_border[end_num] = new QGraphicsPathItem(this);
        // bg_line_border[end_num]->setFlag(QGraphicsItem::ItemStacksBehindParent);
        bg_line_border[end_num]->setPen(border_pen);
        bg_line_border[end_num]->setZValue(-1);

        // * central line
        this->central_line[end_num] = new QGraphicsPathItem(this);
        //central_line[end_num]->setFlag(QGraphicsItem::ItemStacksBehindParent);
        central_line[end_num]->setPen(center_pen);
        central_line[end_num]->setZValue(0.5);

        // * collar lines
        collar_bg_line[end_num] = new QGraphicsPathItem();
        collar_bg_line_border[end_num] = new QGraphicsPathItem();
        collar_central_line[end_num] = new QGraphicsPathItem();

        collar_bg_line[end_num]->setPen(col_edge_pen);
        collar_bg_line_border[end_num]->setPen(col_border_pen);
        collar_central_line[end_num]->setPen(col_center_pen);

        collar_bg_line[end_num]->setZValue(5);
        collar_bg_line_border[end_num]->setZValue(4);
        collar_central_line[end_num]->setZValue(6);
    }

    update_lines();
}

triple_edge_base::triple_edge_base(triple_edge_base &&tr) : EdgeAnchor((EdgeAnchor &&)tr) {
    for( int end_num = 0; end_num < 2; end_num ++) {
        // * background line
        this->bg_line[end_num] = tr.bg_line[end_num];

        // * border lines
        this->bg_line_border[end_num] = tr.bg_line_border[end_num];

        // * central line
        this->central_line[end_num] = tr.central_line[end_num];

        // * collar lines
        collar_bg_line[end_num] = tr.collar_bg_line[end_num];
        collar_bg_line_border[end_num] = tr.collar_bg_line_border[end_num];
        collar_central_line[end_num] = tr.collar_central_line[end_num];
    }

    update_lines();
}

triple_edge_base::~triple_edge_base(){
    delete[] bg_line;
    delete[] bg_line_border;
    delete[] central_line;
    delete[] collar_bg_line;
    delete[] collar_bg_line_border;
    delete[] collar_central_line;
}

QVariant triple_edge_base::itemChange(GraphicsItemChange change, const QVariant &value) {
    QVariant new_value = EdgeAnchor::itemChange(change, value);
    if(scene()) {
        prepareGeometryChange();
        switch(change) {
            case ItemPositionChange:
                update_lines();
                break;

            case ItemPositionHasChanged:
                ends[0]->update_intersections();
                ends[1]->update_intersections();
                update_lines();
                break;

            case ItemSceneHasChanged:
                scene()->addItem(collar_bg_line[0]);
                scene()->addItem(collar_bg_line[1]);
                scene()->addItem(collar_bg_line_border[0]);
                scene()->addItem(collar_bg_line_border[1]);
                scene()->addItem(collar_central_line[0]);
                scene()->addItem(collar_central_line[1]);
                break;

            default:
                // do nothing
                break;
        }
    }

    return new_value;
}


  // // // // // // // // // // // // // // // // // // // // // // //
 // // // // // // // TripleEdgeCustoms // // // // // // // // // // //
// // // // // // // // // // // // // // // // // // // // // // //


triple_edge::triple_edge(Node *n1, Node *n2) : triple_edge_base(n1,n2) {
    QPen center_pen = QPen(QPen(QColor(0,0,0)));
    center_pen.setWidth(_LINE_WIDTH);
    center_pen.setCapStyle(Qt::SquareCap);

    upper_line = new QGraphicsPathItem(this);
    upper_line->setZValue(5);
    upper_line->setPen(center_pen);
    central_line = new QGraphicsPathItem(this);
    central_line->setZValue(5);
    central_line->setPen(center_pen);
    bottom_line = new QGraphicsPathItem(this);
    bottom_line->setZValue(5);
    bottom_line->setPen(center_pen);

    QPen border_pen = QPen(QPen(Qt::cyan));
    border_pen.setWidth(5);

    bg_area1 = new QGraphicsPathItem(this);
    bg_area1->setZValue(3);
    bg_area1->setBrush(Qt::cyan);
    bg_area1->setPen(border_pen);
    bg_area2 = new QGraphicsPathItem(this);
    bg_area2->setZValue(3);
    bg_area2->setBrush(Qt::yellow);
    bg_area2->setPen(border_pen);
}

triple_edge::triple_edge(triple_edge_base &&te) : triple_edge_base((triple_edge_base&&)te) {
    QPen center_pen = QPen(QPen(QColor(0,0,0)));
    center_pen.setWidth(_LINE_WIDTH);
    center_pen.setCapStyle(Qt::SquareCap);

    upper_line = new QGraphicsPathItem(this);
    upper_line->setZValue(5);
    upper_line->setPen(center_pen);
    central_line = new QGraphicsPathItem(this);
    central_line->setZValue(5);
    central_line->setPen(center_pen);
    bottom_line = new QGraphicsPathItem(this);
    bottom_line->setZValue(5);
    bottom_line->setPen(center_pen);


    QPen border_pen = QPen(QPen(Qt::cyan));
    border_pen.setWidth(5);

    bg_area1 = new QGraphicsPathItem(this);
    bg_area1->setZValue(3);
    bg_area1->setBrush(Qt::cyan);
    bg_area1->setPen(border_pen);
    bg_area2 = new QGraphicsPathItem(this);
    bg_area2->setZValue(3);
    bg_area2->setBrush(Qt::yellow);
    bg_area2->setPen(border_pen);
}

triple_edge::~triple_edge() {
    delete upper_line;
    delete central_line;
    delete bottom_line;
}

qreal triple_edge::angle_needed(const qreal r) {
    return 180.0 * std::abs(asin(40.0/r)) / 3.1415926;
}

void triple_edge::paint_middle_rot1(qreal mode, qreal start_angle, qreal end_angle)
{
    QPainterPath cpath, upath, bpath;
    qreal
            bsin = sin(start_angle * 3.1415926/ 180.0),
            msin = sin((start_angle + end_angle)/2 * 3.1415926/ 180.0),
            esin = sin(end_angle * 3.1415926/ 180.0),
            bcos = cos(start_angle * 3.1415926/ 180.0),
            mcos = cos((start_angle + end_angle)/2 * 3.1415926/ 180.0),
            ecos = cos(end_angle * 3.1415926/ 180.0);
    qreal s = position_data.c_right_of * mode > 0 ? 1.0 : -1.0;

    cpath.moveTo(
        position_data.oc.x() + position_data.r * bcos,
        position_data.oc.y() - position_data.r * bsin
    );
    cpath.cubicTo(
        position_data.oc.x() + position_data.r * mcos,
        position_data.oc.y() - position_data.r * msin,
        position_data.oc.x() + (position_data.r + s*_NODE_SIZE/2) * mcos,
        position_data.oc.y() - (position_data.r + s*_NODE_SIZE/2) * msin,
        position_data.oc.x() + (position_data.r + s*_NODE_SIZE/2) * ecos,
        position_data.oc.y() - (position_data.r + s*_NODE_SIZE/2) * esin
    );
    central_line->setPath(cpath);


    upath.moveTo(
        position_data.oc.x() + (position_data.r + s*_NODE_SIZE/2) * bcos,
        position_data.oc.y() - (position_data.r + s*_NODE_SIZE/2) * bsin
    );
    upath.cubicTo(
        position_data.oc.x() + (position_data.r + s*_NODE_SIZE/2) * mcos,
        position_data.oc.y() - (position_data.r + s*_NODE_SIZE/2) * msin,
        position_data.oc.x() + position_data.r * mcos,
        position_data.oc.y() - position_data.r * msin,
        position_data.oc.x() + position_data.r * ecos,
        position_data.oc.y() - position_data.r * esin
    );
    upper_line->setPath(upath);


    bpath.moveTo(
        position_data.oc.x() + (position_data.r - s*_NODE_SIZE/2) * bcos,
        position_data.oc.y() - (position_data.r - s*_NODE_SIZE/2) * bsin
    );
    bpath.cubicTo(
        position_data.oc.x() + (position_data.r - s*_NODE_SIZE/2) * mcos,
        position_data.oc.y() - (position_data.r - s*_NODE_SIZE/2) * msin,
        position_data.oc.x() + (position_data.r - s*_NODE_SIZE/2) * mcos,
        position_data.oc.y() - (position_data.r - s*_NODE_SIZE/2) * msin,
        position_data.oc.x() + (position_data.r - s*_NODE_SIZE/2) * ecos,
        position_data.oc.y() - (position_data.r - s*_NODE_SIZE/2) * esin
    );
    bottom_line->setPath(bpath);

    upath.connectPath(bpath.toReversed());
    cpath.connectPath(bpath.toReversed());
    bg_area1->setPath(upath);
    bg_area2->setPath(cpath);
}

void triple_edge::paint_middle(qreal start_angle, qreal end_angle) {
    switch (type) {
    case EdgeType::Base:
    {
        QPainterPath path;
        upper_line->setPath(path);
        central_line->setPath(path);
        bottom_line->setPath(path);
        bg_area1->setPath(path);
        bg_area2->setPath(path);
        return;
    }

    case EdgeType::Rot132:
        paint_middle_rot1(1.0, start_angle, end_angle);
        return;

    case EdgeType::Rot213:
        paint_middle_rot1(-1.0, start_angle, end_angle);
        return;
    }
}

void triple_edge::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::RightButton) {
        QMenu menu;
        QAction *a1 = menu.addAction("Base");
        QAction *a2 = menu.addAction("Rot132");
        QAction *a3 = menu.addAction("Rot213");
        QAction *res = menu.exec(event->screenPos());

        if( res == a1) {
            type = Base;
            triple_edge_base::itemChange(ItemPositionHasChanged, triple_edge::pos());
        } else if( res == a2) {
            type = Rot132;
            triple_edge_base::itemChange(ItemPositionHasChanged, triple_edge::pos());
        } else if (res == a3) {
            type = Rot213;
            triple_edge_base::itemChange(ItemPositionHasChanged, triple_edge::pos());
        }
    }

    return triple_edge_base::mouseReleaseEvent(event);
}












// (a'',b) (b'',b) (c'',b) (d'',b) (e'',b) (f'',b) (g'',b) (h'',b) (k'',b) (l'',b) (m'',b) (n'',b) (o'',b) (p'',b) (r'',b) (s'',b) (t'',b) (u'',b) (x'',b) (z'',b)
// (a,b) (a,c) (a,d) (a,e) (b,c) (b,d) (b,e) (c,d) (c,e) (d,e)
