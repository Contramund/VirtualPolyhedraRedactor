#include "node.h"

#include "edgeanchor.h"
#include "triple_edges.h"
#include "geometric_functions.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QDebug>

#include <math.h>

void Node::update_intersections()
{
   // Update data of intersections
   for(int i = 0; i < common_edges->size(); i++) {
        for( int j = i+1; j < common_edges->size(); j++){
            triple_edge_base* I = common_edges->at(i);
            triple_edge_base* J = common_edges->at(j);
            if( I != nullptr && J != nullptr) {
                QPointF ans1, ans2;

                qreal sign_i = (I->position_data.c_right_of >= 0 ? 1 : -1) * (I->ends[1] == this ? 1 : -1);
                qreal sign_j = (J->position_data.c_right_of >= 0 ? 1 : -1) * (J->ends[0] == this ? 1 : -1);

                // If there are loops, so the sign calculates wrong
                // Quick fix:
                if( I->ends[0] == I->ends[1]) {
                    if( i + 1 < common_edges->size()) {
                        if(common_edges->at(i+1) == I) {
                            sign_i *= -1;
                        }
                    }
                }
                if(J->ends[1] == J->ends[0] && j !=0 ) {
                    if( common_edges->at(j-1) == J) {
                        sign_j *= -1;
                    }
                }

                QPointF r1 = (pos() - I->position_data.O);
                r1 = r1/ sqrt(QPointF::dotProduct(r1,r1));
                r1 = QPointF(sign_i * r1.y(), - sign_i * r1.x());
                QPointF r2 = (pos() - J->position_data.O);
                r2 = r2/ sqrt(QPointF::dotProduct(r2,r2));
                r2 = QPointF( - sign_j * r2.y(), sign_j * r2.x());

                // Check for coincining centres
                QPointF IJ =  I->position_data.O - J->position_data.O;
                if(QPointF::dotProduct(IJ,IJ) < 10) {
                    (*border_intersection)[std::make_pair(i,j)] = pos();
                    continue;
                }

                // Check for tangent circles
                qreal clockwise_cos = sign_i * (r1.x() * r2.y() - r1.y() * r2.x());
                if( abs(clockwise_cos) <= 0.02 ) {
                    (*border_intersection)[std::make_pair(i,j)] = pos();
                    continue;
                }

                if( sign_i == sign_j) {
                    if( clockwise_cos <= 0 ) {
                        third_vertex(I->position_data.r-_NODE_SIZE/2-_LINE_WIDTH/2, J->position_data.r-_NODE_SIZE/2-_LINE_WIDTH/2, J->position_data.O, I->position_data.O, ans1, ans2);
                        QPointF ans_m_m = QPointF::dotProduct(ans1-pos(), ans1-pos()) <= QPointF::dotProduct(ans2-pos(), ans2-pos()) ? ans1 : ans2;
                        (*border_intersection)[std::make_pair(i,j)] = ans_m_m;
                    } else {
                        third_vertex(I->position_data.r+_NODE_SIZE/2+_LINE_WIDTH/2, J->position_data.r+_NODE_SIZE/2+_LINE_WIDTH/2, J->position_data.O, I->position_data.O, ans1, ans2);
                        QPointF ans_p_p = QPointF::dotProduct(ans1-pos(), ans1-pos()) <= QPointF::dotProduct(ans2-pos(), ans2-pos()) ? ans1 : ans2;
                        (*border_intersection)[std::make_pair(i,j)] = ans_p_p;
                    }
                } else {
                    if( clockwise_cos <= 0 ) {
                        third_vertex(I->position_data.r-_NODE_SIZE/2-_LINE_WIDTH/2, J->position_data.r+_NODE_SIZE/2+_LINE_WIDTH/2, J->position_data.O, I->position_data.O, ans1, ans2);
                        QPointF ans_m_p = QPointF::dotProduct(ans1-pos(), ans1-pos()) <= QPointF::dotProduct(ans2-pos(), ans2-pos()) ? ans1 : ans2;
                        (*border_intersection)[std::make_pair(i,j)] = ans_m_p;
                    } else {
                        third_vertex(I->position_data.r+_NODE_SIZE/2+_LINE_WIDTH/2, J->position_data.r-_NODE_SIZE/2-_LINE_WIDTH/2, J->position_data.O, I->position_data.O, ans1, ans2);
                        QPointF ans_p_m = QPointF::dotProduct(ans1-pos(), ans1-pos()) <= QPointF::dotProduct(ans2-pos(), ans2-pos()) ? ans1 : ans2;
                        (*border_intersection)[std::make_pair(i,j)] = ans_p_m;
                    }
                }
           }
       }
   }

   // Update all collors and edges
   for(int i = 0; i < common_edges->size(); i++) {
        triple_edge_base* I = common_edges->at(i);
        if( I != nullptr ) {
            I->update_collars();
        }
   }
}

Node::Node(const QRectF &rect) : QGraphicsEllipseItem(rect) {
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setBrush(QBrush(QColor(200,0,0)));
    setPen(QPen(QBrush(QColor(0,0,0)), _LINE_WIDTH));
    QGraphicsItem::setZValue(0);
    common_edges = new std::vector<triple_edge_base *>;
    common_nodes = new std::vector<Node *>;
    border_intersection = new std::map<std::pair<int,int>,QPointF>;
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value) {
    if(scene()) {
        switch(change) {
            case ItemPositionChange:
                // update_edges(value);
                return QGraphicsItem::itemChange(change, value);

            case ItemPositionHasChanged:
                update_intersections();
                for( int iter = 0; iter < (int)common_edges->size(); iter++) {
                    common_edges->at(iter)->setPos(0,0);
                }
                return QGraphicsItem::itemChange(change, value);

            default:
                return QGraphicsItem::itemChange(change, value);
        }
    } else {
        return QGraphicsItem::itemChange(change, value);
    }
}



