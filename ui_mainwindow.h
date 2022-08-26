/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionSave_Picture;
    QAction *SwitchSidePanel;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QGraphicsView *Plane;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *GraphStr;
    QPushButton *pushButton;
    QMenuBar *menubar;
    QMenu *menuFIles;
    QMenu *menuView;
    QStatusBar *statusbar;
    QDockWidget *SidePanel;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *Nodes;
    QHBoxLayout *horizontalLayout_2;
    QListWidget *NodeList;
    QWidget *Edges;
    QVBoxLayout *verticalLayout_3;
    QTreeView *ListObjects;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1129, 625);
        actionSave_Picture = new QAction(MainWindow);
        actionSave_Picture->setObjectName(QString::fromUtf8("actionSave_Picture"));
        SwitchSidePanel = new QAction(MainWindow);
        SwitchSidePanel->setObjectName(QString::fromUtf8("SwitchSidePanel"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        Plane = new QGraphicsView(centralwidget);
        Plane->setObjectName(QString::fromUtf8("Plane"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Plane->sizePolicy().hasHeightForWidth());
        Plane->setSizePolicy(sizePolicy);

        verticalLayout_2->addWidget(Plane);

        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        GraphStr = new QLineEdit(groupBox);
        GraphStr->setObjectName(QString::fromUtf8("GraphStr"));

        horizontalLayout->addWidget(GraphStr);

        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout->addWidget(pushButton);


        verticalLayout_2->addWidget(groupBox);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1129, 20));
        menuFIles = new QMenu(menubar);
        menuFIles->setObjectName(QString::fromUtf8("menuFIles"));
        menuView = new QMenu(menubar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
        SidePanel = new QDockWidget(MainWindow);
        SidePanel->setObjectName(QString::fromUtf8("SidePanel"));
        SidePanel->setEnabled(true);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        dockWidgetContents->setEnabled(true);
        verticalLayout = new QVBoxLayout(dockWidgetContents);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tabWidget = new QTabWidget(dockWidgetContents);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setElideMode(Qt::ElideNone);
        Nodes = new QWidget();
        Nodes->setObjectName(QString::fromUtf8("Nodes"));
        sizePolicy.setHeightForWidth(Nodes->sizePolicy().hasHeightForWidth());
        Nodes->setSizePolicy(sizePolicy);
        Nodes->setLayoutDirection(Qt::LeftToRight);
        Nodes->setAutoFillBackground(true);
        horizontalLayout_2 = new QHBoxLayout(Nodes);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        NodeList = new QListWidget(Nodes);
        NodeList->setObjectName(QString::fromUtf8("NodeList"));

        horizontalLayout_2->addWidget(NodeList);

        tabWidget->addTab(Nodes, QString());
        Edges = new QWidget();
        Edges->setObjectName(QString::fromUtf8("Edges"));
        verticalLayout_3 = new QVBoxLayout(Edges);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        ListObjects = new QTreeView(Edges);
        ListObjects->setObjectName(QString::fromUtf8("ListObjects"));

        verticalLayout_3->addWidget(ListObjects);

        tabWidget->addTab(Edges, QString());

        verticalLayout->addWidget(tabWidget);

        SidePanel->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), SidePanel);

        menubar->addAction(menuFIles->menuAction());
        menubar->addAction(menuView->menuAction());
        menuFIles->addAction(actionSave_Picture);
        menuView->addAction(SwitchSidePanel);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        actionSave_Picture->setText(QApplication::translate("MainWindow", "Save Picture", nullptr));
        SwitchSidePanel->setText(QApplication::translate("MainWindow", "Side Panel", nullptr));
#ifndef QT_NO_SHORTCUT
        SwitchSidePanel->setShortcut(QApplication::translate("MainWindow", "Ctrl+P", nullptr));
#endif // QT_NO_SHORTCUT
        groupBox->setTitle(QApplication::translate("MainWindow", "Actions", nullptr));
        label->setText(QApplication::translate("MainWindow", "Insert Polyedra", nullptr));
        GraphStr->setText(QApplication::translate("MainWindow", "(a,b)", nullptr));
        pushButton->setText(QApplication::translate("MainWindow", "Compute", nullptr));
#ifndef QT_NO_SHORTCUT
        pushButton->setShortcut(QApplication::translate("MainWindow", "Return", nullptr));
#endif // QT_NO_SHORTCUT
        menuFIles->setTitle(QApplication::translate("MainWindow", "FIles", nullptr));
        menuView->setTitle(QApplication::translate("MainWindow", "View", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(Nodes), QApplication::translate("MainWindow", "Nodes", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(Edges), QApplication::translate("MainWindow", "Edges", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
