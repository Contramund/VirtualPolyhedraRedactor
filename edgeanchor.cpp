#include "edgeanchor.h"

#include "geometric_functions.h"
#include "properties.h"

#include <QGraphicsScene>
#include <QDebug>

#include <math.h>

  // // // // // // // // // // // // // // // // // // // // // // //
 // // // // // // // EdgeData // // // // // // // // // // // // //
// // // // // // // // // // // // // // // // // // // // // // //

void EdgePositionData::update(QPointF a, QPointF b, QPointF c) {
    EdgePositionData::a = a;
    EdgePositionData::b = b;
    EdgePositionData::c = c;
    ab_len = sqrt(QPointF::dotProduct(a-b,a-b));
    mid_p = a + (b-a)/2;
    ac_len2 = QPointF::dotProduct(a-c, a-c);
    mc_len = sqrt(QPointF::dotProduct(mid_p-c, mid_p-c));
    oc_len = ac_len2 / (2*mc_len);
    oc = (mid_p - c) * oc_len / mc_len;
    r = sqrt(QPointF::dotProduct(oc,oc));
    O = oc+c;
    c_right_of = (a.x()-mid_p.x())*(c.y()-mid_p.y()) -
                    (a.y()-mid_p.y())*(c.x()-mid_p.x()) > 0 ? 1 : -1;
    o_right_of = (a.x()-mid_p.x())*(O.y()-mid_p.y()) -
                    (a.y()-mid_p.y())*(O.x()-mid_p.x());
    QPointF beg = O - a;
    QPointF e = O - b;
    arc_angle = std::abs(std::atan2(beg.x()*e.y() - beg.y()*e.x(), beg.x()*e.x() + beg.y()*e.y()) * 180.0 / 3.1415926);
    start_angle =  angle_to(
                    a - O,
                    QPointF(1.0, 0.0));
    oriented_angle =  (c_right_of < 0 ? 1 : -1) *
                    ( (o_right_of > 0) ^ (c_right_of > 0) ? arc_angle : 360 - arc_angle );
}

EdgePositionData::EdgePositionData(QPointF a, QPointF b, QPointF c) {
    update(a,b,c);
}

EdgePositionData::EdgePositionData() {}

  // // // // // // // // // // // // // // // // // // // // // // //
 // // // // // // // Edge Anchor // // // // // // // // // // // //
// // // // // // // // // // // // // // // // // // // // // // //

EdgeAnchor::EdgeAnchor( Node *n1, Node *n2)
    : QGraphicsEllipseItem(-_NODE_SIZE/2, -_NODE_SIZE/2, _NODE_SIZE, _NODE_SIZE)
{
    // Set flags
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
    // setBrush(QBrush(QColor(200,200,200)));
    setBrush(Qt::NoBrush);
    setPen(Qt::NoPen);
    this->setZValue(1);

    // Set data
    ends[0] = n1;
    ends[1] = n2;
    QPointF p1 = ends[0]->pos();
    QPointF p2 = ends[1]->pos();

    if( p1 != p2) {
        QPointF mid_p = p1 + (p2-p1)/2;
        QPointF vec_v_half = (p2-p1)/2;
        QPointF perp_v(vec_v_half.y(), -vec_v_half.x());

        QGraphicsEllipseItem::setPos(mid_p + perp_v);
        position_data = EdgePositionData(p1, p2, mid_p + perp_v);
        coeff = position_data.c_right_of * position_data.mc_len / position_data.ab_len;
    } else {
        QGraphicsEllipseItem::setPos(ends[0]->pos() + QPointF(2*_NODE_SIZE, 0));
        position_data = EdgePositionData(p1, p2, ends[0]->pos() + QPointF(2*_NODE_SIZE, 0));
    }
}

EdgeAnchor::EdgeAnchor(EdgeAnchor &&ea) : QGraphicsEllipseItem(ea.rect(), ea.parentItem()) {
    // Set data
    ends[0] = ea.ends[0];
    ends[1] = ea.ends[1];

    position_data = ea.position_data;
    coeff = ea.coeff;
}

EdgeAnchor::~EdgeAnchor() {}

void EdgeAnchor::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    isMovingByCursor = true;
    setBrush(QBrush(QColor(70,70,70)));
    QGraphicsItem::mousePressEvent(event);
}

void EdgeAnchor::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    isMovingByCursor = false;
    setBrush(QBrush(QColor(200,200,200)));
    //setBrush(Qt::NoBrush);
    QGraphicsItem::mouseReleaseEvent(event);
}

QVariant EdgeAnchor::itemChange(GraphicsItemChange change, const QVariant &value) {
    if(scene()) {
        switch(change) {
            case ItemPositionChange:
            {
                QPointF n1 = ends[0]->pos();
                QPointF n2 = ends[1]->pos();
                if( n1 == n2 ) {
                    return isMovingByCursor ? value : 2*position_data.oc;
                } else if(isMovingByCursor) {
                    // value is the new position.
                    QPointF newPos = value.toPointF();

                    // projection to middle ortogonal line
                    QPointF mid_p = n1 + (n2-n1)/2;
                    QPointF vec_v_norm = (n2-n1)/sqrt(QPointF::dotProduct(n2-n1, n2-n1));
                    QPointF perp_v(vec_v_norm.y(), -vec_v_norm.x());
                    QPointF mid_to_pos_v = newPos - mid_p;

                    return mid_p + QPointF::dotProduct(perp_v, mid_to_pos_v) * perp_v;
                } else {
                    // updating position proportional to last move by cursor
                    QPointF ab = n2 - n1;
                    QPointF perp(ab.y(), -ab.x());
                    QPointF mid_p = n1 + (n2-n1)/2;

                    return mid_p + coeff * perp;
                }
            }

            case ItemPositionHasChanged:
                position_data.update(ends[0]->pos(), ends[1]->pos(), pos());
                if(isMovingByCursor) {
                    coeff = position_data.c_right_of * position_data.mc_len / position_data.ab_len;
                }
                return QGraphicsItem::itemChange(change, value);

            default:
                return QGraphicsItem::itemChange(change, value);
        }
    } else {
        return QGraphicsItem::itemChange(change, value);
    }
}
