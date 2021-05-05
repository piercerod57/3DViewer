#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ModelViewer.h"

class ModelViewer : public QMainWindow
{
    Q_OBJECT

public:
    ModelViewer(QWidget *parent = Q_NULLPTR);

//public slots:

private:
    Ui::ModelViewerClass ui;
};
