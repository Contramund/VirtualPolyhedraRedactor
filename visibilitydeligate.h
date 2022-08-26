#ifndef VISIBILITYDELIGATE_H
#define VISIBILITYDELIGATE_H

#include <QObject>
#include <QItemDelegate>

class visibilityDeligate : public QItemDelegate
{
public:
    visibilityDeligate(QObject* parent);

public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
};

#endif // VISIBILITYDELIGATE_H
