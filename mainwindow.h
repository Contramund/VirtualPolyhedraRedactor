#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QListWidgetItem>
#include <QtCore>
#include <QtGui>

#include "node.h"
#include "edgeanchor.h"
#include "triple_edges.h"
#include "objectstreemodel.h"
#include "visibilitydeligate.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    std::vector<triple_edge *> AnchorLinks;
    // std::vector<EdgeAnchor *> AnchorLinks;
    std::map<std::string,int> NodeNames;
    std::vector<Node *> NodeLinks;

private slots:
    void on_pushButton_clicked();

    void on_SwitchSidePanel_triggered();

    void on_NodeList_itemChanged(QListWidgetItem *item);

private:
    QGraphicsScene *scene;
    Ui::MainWindow *ui;
    ObjectTreeModel* model;
};
#endif // MAINWINDOW_H
