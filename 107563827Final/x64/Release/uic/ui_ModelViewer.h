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
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ModelViewerClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ModelViewerClass)
    {
        if (ModelViewerClass->objectName().isEmpty())
            ModelViewerClass->setObjectName(QString::fromUtf8("ModelViewerClass"));
        ModelViewerClass->resize(600, 400);
        menuBar = new QMenuBar(ModelViewerClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        ModelViewerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ModelViewerClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        ModelViewerClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(ModelViewerClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        ModelViewerClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(ModelViewerClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        ModelViewerClass->setStatusBar(statusBar);

        retranslateUi(ModelViewerClass);

        QMetaObject::connectSlotsByName(ModelViewerClass);
    } // setupUi

    void retranslateUi(QMainWindow *ModelViewerClass)
    {
        ModelViewerClass->setWindowTitle(QApplication::translate("ModelViewerClass", "ModelViewer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ModelViewerClass: public Ui_ModelViewerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODELVIEWER_H
