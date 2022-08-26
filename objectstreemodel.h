#ifndef OBJECTSTREEMODEL_H
#define OBJECTSTREEMODEL_H


#include <QAbstractItemModel>
#include <QObject>

class ObjectTreeModel : public QAbstractItemModel
{
  Q_OBJECT
public:
  ObjectTreeModel(QObject* parent = nullptr);
  void setColumns(QStringList cols);
  void addItem(QObject* item, const QModelIndex &parentIdx);

protected:
  QStringList _columns;
  QObject* _rootItem;
  QObject* objByIndex(const QModelIndex &index) const;

public:
  virtual QModelIndex index(int row, int column, const QModelIndex &parent) const override;
  virtual QModelIndex parent(const QModelIndex &child) const override;
  virtual int rowCount(const QModelIndex &parent) const override;
  virtual int columnCount(const QModelIndex &parent) const override;
  virtual QVariant data(const QModelIndex &index, int role) const override;
};

#endif // OBJECTSTREEMODEL_H
