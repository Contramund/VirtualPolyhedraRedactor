#include "objectstreemodel.h"

#include <QDebug>
#include <QAbstractItemModel>

ObjectTreeModel::ObjectTreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    _rootItem = new QObject(this);
}

void ObjectTreeModel::setColumns(QStringList cols)
{
    _columns = cols;
}

void ObjectTreeModel::addItem(QObject *item, const QModelIndex &parentIdx)
{
    beginInsertRows(parentIdx, rowCount(parentIdx), rowCount(parentIdx));
    item->setParent(objByIndex(parentIdx));
    endInsertRows();
}

QObject *ObjectTreeModel::objByIndex(const QModelIndex &index) const
{
    if (!index.isValid())
        return _rootItem;
    return static_cast<QObject*>(index.internalPointer());
}

QModelIndex ObjectTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();
    QObject* parentObj = objByIndex(parent);
    return createIndex(row, column, parentObj->children().at(row));
}

QModelIndex ObjectTreeModel::parent(const QModelIndex &child) const
{
    QObject* childObj = objByIndex(child);
    QObject* parentObj = childObj->parent();

    if (parentObj == _rootItem)
        return QModelIndex();

    QObject* grandParentObj = parentObj->parent();
    int row = grandParentObj->children().indexOf(parentObj);

    return createIndex(row, 0, parentObj);
}

int ObjectTreeModel::rowCount(const QModelIndex &parent) const
{
    return objByIndex(parent)->children().count();
}

int ObjectTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return _columns.count();
}

QVariant ObjectTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        return objByIndex(index)->property(_columns.at(index.column()).toUtf8());
    }

    return QVariant();
}
