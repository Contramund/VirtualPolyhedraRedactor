#ifndef GEOMETRIC_FUNCTIONS_H
#define GEOMETRIC_FUNCTIONS_H

#include <QPointF>


// finds third vertex of triangle by lenghts of three sides and coordinates of two vertexes
// stable to degeneration of triangle with non-null sides, but unstable to degeneration of triangle's sides
void third_vertex(
        const qreal &a,
        const qreal &b,
        const QPointF &A,
        const QPointF &B,
        QPointF &ans1,
        QPointF &ans2);

qreal angle_to( const QPointF &b, const QPointF &e);

bool arcsIntersects(const QPointF &O_1, const qreal &r_1, const qreal &start_angle_1, const qreal &end_angle_1,
                    const QPointF &O_2, const qreal &r_2, const qreal &start_angle_2, const qreal &end_angle_2);

bool boldArcsIntersects(const QPointF &O_1, const qreal &r_1, const qreal &start_angle_1, const qreal &end_angle_1,
                    const QPointF &O_2, const qreal &r_2, const qreal &start_angle_2, const qreal &end_angle_2);

#endif // GEOMETRIC_FUNCTIONS_H
