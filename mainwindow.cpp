#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QListWidgetItem>
#include <Qt>
#include <QComboBox>
#include <QStandardItemModel>
#include <QGraphicsView>
#include <QOpenGLWidget>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set scene
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0,0,1000,1000);
    ui->Plane->setViewport(new QOpenGLWidget());
    ui->Plane->setScene(scene);
    ui->Plane->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    // Set old side pannel
    ui->SidePanel->setVisible(false);

    // Set new side pannel
    model = new ObjectTreeModel(this);

    QStringList cols;
    cols << "Name" << "isVisible" << "Option";
    model->setColumns(cols);

    ui->ListObjects->setModel(model);
    visibilityDeligate* vis_deligator = new visibilityDeligate(this);
    ui->ListObjects->setItemDelegateForColumn(1, vis_deligator);
    ui->ListObjects->setColumnWidth(0,70);
    ui->ListObjects->setColumnWidth(1,40);
    ui->ListObjects->setHeaderHidden(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    // Update graph variables
    QString input_text = ui->GraphStr->text();
    QStringList edges = input_text.split(QRegExp("[\\s]"), QString::SkipEmptyParts);
    for( auto s: edges) {
        QStringList edge = s.split(QRegExp("[\\s\\n(),]"), QString::SkipEmptyParts);
        if( edge.count() == 2) {
            std::string e1 = edge[0].toStdString();
            std::string e2 = edge[1].toStdString();

            Node *pt1;
            if( NodeNames.find(e1) == NodeNames.end()) {
                NodeNames.insert(std::make_pair(e1, NodeNames.size()));

                pt1 = new Node(QRectF(-_NODE_SIZE/2, -_NODE_SIZE/2, _NODE_SIZE, _NODE_SIZE));
                pt1->setPos(NodeNames.size() * 2 * _NODE_SIZE, NodeNames.size() * 2 * _NODE_SIZE);
                scene->addItem(pt1);
                NodeLinks.push_back(pt1);

                // add old data
                QListWidgetItem *p_item = new QListWidgetItem;
                p_item->setText(edge[0]);
                p_item->setCheckState(Qt::Checked);
                QComboBox* widget = new QComboBox();
                widget->addItems(QStringList(QList<QString>{"1","2","3"}));
                ui->NodeList->addItem(p_item);
                ui->NodeList->setItemWidget(p_item, widget);

                // add new data
                QObject* item = new QObject();
                item->setProperty("Name", QString::fromStdString(e1));
                item->setProperty("isVisible", "Hide");
                model->addItem(item, QModelIndex());
            } else {
                pt1 = NodeLinks[NodeNames[e1]];
            }

            Node *pt2;
            if( NodeNames.find(e2) == NodeNames.end()) {
                NodeNames.insert(std::make_pair(e2, NodeNames.size()));

                pt2 = new Node(QRectF(-_NODE_SIZE/2, -_NODE_SIZE/2, _NODE_SIZE, _NODE_SIZE));
                pt2->setPos(NodeNames.size() * 2 * _NODE_SIZE, NodeNames.size() * 2 * _NODE_SIZE);
                scene->addItem(pt2);
                NodeLinks.push_back(pt2);

                // add old data
                QListWidgetItem *p_item = new QListWidgetItem;
                p_item->setText(edge[1]);
                p_item->setCheckState(Qt::Checked);
                QComboBox* widget = new QComboBox();
                widget->addItems(QStringList(QList<QString>{"1","2","3"}));
                ui->NodeList->addItem(p_item);
                ui->NodeList->setItemWidget(p_item, widget);

                // add new data
                QObject* item = new QObject();
                item->setProperty("Name", QString::fromStdString(e2));
                item->setProperty("isVisible", "Hide");
                model->addItem(item, QModelIndex());
            } else {
                pt2 = NodeLinks[NodeNames[e2]];
            }

            // auto edge = new EdgeAnchor(pt1, pt2);
            auto edge = new triple_edge(pt1, pt2);
            AnchorLinks.push_back(edge);
            pt1->common_nodes->push_back(pt2);
            pt1->common_edges->push_back(edge);
            pt1->childItems().push_back(edge);
            pt2->common_nodes->push_back(pt1);
            pt2->common_edges->push_back(edge);
            pt2->childItems().push_back(edge);
            scene->addItem(edge);

            statusBar()->showMessage("Got you");
        } else {
            statusBar()->showMessage("Error parsing the line");
        }
    }
}

void MainWindow::on_SwitchSidePanel_triggered() {
    ui->SidePanel->setVisible(!ui->SidePanel->isVisible());
}

void MainWindow::on_NodeList_itemChanged(QListWidgetItem *item) {
    Node *n = NodeLinks.at(NodeNames.at(item->text().toStdString()));
    bool is_visible = item->checkState() == Qt::Checked ? 1 : 0;

    n->setVisible(is_visible);
    for(auto i : *n->common_edges ) {
        i->setVisible(is_visible);
    }
}
