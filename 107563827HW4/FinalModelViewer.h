#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_FinalModelViewer.h"


class QTimer;

class FinalModelViewer : public QMainWindow
{
    Q_OBJECT

public:
    FinalModelViewer(QWidget *parent = Q_NULLPTR);

	

public slots:
	void loadFromFile();


private:
    Ui::FinalModelViewerClass ui;
};
