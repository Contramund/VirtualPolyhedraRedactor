#include "geometric_functions.h"

#include "properties.h"

#include <QtGlobal>
#include <QtMath>
#include <QPainterPath>
#include <QDebug>

#include <math.h>

void third_vertex(const qreal &a, const qreal &b, const QPointF &A, const QPointF &B, QPointF &ans1, QPointF &ans2)
{
    QPointF AB = B - A;
    qreal c = sqrt(QPointF::dotProduct(AB, AB));
    QPointF AB_norm = AB / c;

    qreal proj_C_on_c = (b*b + c*c - a*a)/ (2 * c);
    QPointF AB_perp_norm = QPointF(AB_norm.y(), -AB_norm.x());
    QPointF P = A + AB_norm * proj_C_on_c;
    qreal bc_len2 =  b * b - proj_C_on_c * proj_C_on_c;

    // check for triangle's degenerateness
    if( abs(a + b - c) <= 7.0 ) bc_len2 = abs(bc_len2);
    if( abs(c + a - b) <= 7.0 ) bc_len2 = abs(bc_len2);
    if( abs(b + c - a) <= 7.0 ) bc_len2 = abs(bc_len2);

    QPointF CP = AB_perp_norm * sqrt(bc_len2);
    ans1 = P + CP;
    ans2 = P - CP;
}

qreal angle_to( const QPointF &b, const QPointF &e) {
    qreal ans = qAtan2(b.x()*e.y() - b.y()*e.x(), b.x()*e.x() + b.y()*e.y()) * 180.0 / 3.1415926;
    return ans;
}

bool angleInInterval(qreal &&angle, const qreal &start, const qreal &end) {
    qreal l = std::min(start, end);
    qreal r = std::max(start, end);
    while( angle < l ) {
        angle += 360.0;
    }
    while( angle > r ) {
        angle -= 360.0;
    }

    return angle >= l ? true : false;
}

bool arcsIntersects(const QPointF &O_1, const qreal &r_1, const qreal &start_angle_1, const qreal &end_angle_1,
                    const QPointF &O_2, const qreal &r_2, const qreal &start_angle_2, const qreal &end_angle_2)
{
    QPointF ans[2];
    third_vertex(r_1, r_2, O_2, O_1, ans[0], ans[1]);

    for( int arc_num = 0; arc_num < 2; arc_num ++) {
        if( angleInInterval(angle_to(ans[arc_num] - O_1, QPointF(1.0,0.0)), start_angle_1, start_angle_1 + end_angle_1) &&
            angleInInterval(angle_to(ans[arc_num] - O_2, QPointF(1.0,0.0)), start_angle_2, start_angle_2 + end_angle_2))
        {
            return true;
        }
    }

    return false;
}



bool boldArcsIntersects(const QPointF &O_1, const qreal &r_1, const qreal &start_angle_1, const qreal &end_angle_1, const QPointF &O_2, const qreal &r_2, const qreal &start_angle_2, const qreal &end_angle_2)
{
    bool ans = false;
    ans |= arcsIntersects(O_1, r_1+_NODE_SIZE/2+_LINE_WIDTH/2, start_angle_1, end_angle_1, O_2, r_2+_NODE_SIZE/2+_LINE_WIDTH/2, start_angle_2, end_angle_2);
    ans |= arcsIntersects(O_1, r_1-_NODE_SIZE/2-_LINE_WIDTH/2, start_angle_1, end_angle_1, O_2, r_2-_NODE_SIZE/2-_LINE_WIDTH/2, start_angle_2, end_angle_2);
    ans |= arcsIntersects(O_1, r_1+_NODE_SIZE/2+_LINE_WIDTH/2, start_angle_1, end_angle_1, O_2, r_2-_NODE_SIZE/2-_LINE_WIDTH/2, start_angle_2, end_angle_2);
    ans |= arcsIntersects(O_1, r_1-_NODE_SIZE/2-_LINE_WIDTH/2, start_angle_1, end_angle_1, O_2, r_2+_NODE_SIZE/2+_LINE_WIDTH/2, start_angle_2, end_angle_2);
    ans |= arcsIntersects(O_1, r_1, start_angle_1, end_angle_1, O_2, r_2, start_angle_2, end_angle_2);

    return ans;
    /*
    QPainterPath p1,p1_1, p2, p2_1;
    qreal gauge = _NODE_SIZE/2 + _LINE_WIDTH/2;

    p1.arcMoveTo( O_1.x()-r_1+gauge, O_1.y()-r_1+gauge, 2*(r_1-gauge), 2*(r_1-gauge), start_angle_1);
    p1.arcTo( O_1.x()-r_1+gauge, O_1.y()-r_1+gauge, 2*(r_1-gauge), 2*(r_1-gauge), start_angle_1, end_angle_1);
    p1_1.arcMoveTo( O_1.x()-r_1-gauge, O_1.y()-r_1-gauge, 2*(r_1+gauge), 2*(r_1+gauge), start_angle_1 + end_angle_1);
    p1_1.arcTo( O_1.x()-r_1-gauge, O_1.y()-r_1-gauge, 2*(r_1+gauge), 2*(r_1+gauge), start_angle_1 + end_angle_1, - end_angle_1);
    p1.connectPath(p1_1);

    p2.arcMoveTo( O_2.x()-r_2+gauge, O_2.y()-r_2+gauge, 2*(r_2-gauge), 2*(r_2-gauge), start_angle_2);
    p2.arcTo( O_2.x()-r_2+gauge, O_2.y()-r_2+gauge, 2*(r_2-gauge), 2*(r_2-gauge), start_angle_2, end_angle_2);
    p2_1.arcMoveTo( O_2.x()-r_2-gauge, O_2.y()-r_2-gauge, 2*(r_2+gauge), 2*(r_2+gauge), start_angle_2 + end_angle_2);
    p2_1.arcTo( O_2.x()-r_2-gauge, O_2.y()-r_2-gauge, 2*(r_2+gauge), 2*(r_2+gauge), start_angle_2 + end_angle_2, - end_angle_2);
    p2.connectPath(p2_1);

    return p1.intersects(p2);
    */
}
