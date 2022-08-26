#include "visibilitydeligate.h"

#include <QPushButton>
#include <QDebug>
#include <QPainter>
#include <QApplication>


visibilityDeligate::visibilityDeligate(QObject* parent) : QItemDelegate(parent) {}

void visibilityDeligate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QPushButton button(index.data().toString());
    button.setGeometry(option.rect);
    painter->save();
    painter->translate(option.rect.topLeft());
    button.render(painter);
    painter->restore();
}

bool visibilityDeligate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) {
    if( event->type() == QEvent::MouseButtonRelease) {
        QObject* button = static_cast<QObject*>(index.internalPointer());
        QString new_text = index.data().toString() == "Show" ? "Hide" : "Show";
        button->setProperty("isVisible", new_text);
    }

    return true;
}

