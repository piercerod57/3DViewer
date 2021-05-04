/********************************************************************************
** Form generated from reading UI file 'ModelViewer.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODELVIEWER_H
#define UI_MODELVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QOpenGLWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ModelViewerClass
{
public:
    QWidget *centralWidget;
    QOpenGLWidget *openGLWidget;
    QPushButton *pushButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ModelViewerClass)
    {
        if (ModelViewerClass->objectName().isEmpty())
            ModelViewerClass->setObjectName(QString::fromUtf8("ModelViewerClass"));
        ModelViewerClass->resize(1251, 585);
        centralWidget = new QWidget(ModelViewerClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        openGLWidget = new QOpenGLWidget(centralWidget);
        openGLWidget->setObjectName(QString::fromUtf8("openGLWidget"));
        openGLWidget->setGeometry(QRect(10, 10, 631, 521));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(720, 510, 75, 23));
        ModelViewerClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ModelViewerClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1251, 21));
        ModelViewerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ModelViewerClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        ModelViewerClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ModelViewerClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        ModelViewerClass->setStatusBar(statusBar);

        retranslateUi(ModelViewerClass);

        QMetaObject::connectSlotsByName(ModelViewerClass);
    } // setupUi

    void retranslateUi(QMainWindow *ModelViewerClass)
    {
        ModelViewerClass->setWindowTitle(QApplication::translate("ModelViewerClass", "ModelViewer", nullptr));
        pushButton->setText(QApplication::translate("ModelViewerClass", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ModelViewerClass: public Ui_ModelViewerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODELVIEWER_H
