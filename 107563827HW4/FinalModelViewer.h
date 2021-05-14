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
	void light1_enabled(int status);
	void light2_enabled(int status);
	void light3_enabled(int status);
	
	void light1_posx(double level);
	void light1_posy(double level);
	void light1_posz(double level);

	void light2_posx(double level);
	void light2_posy(double level);
	void light2_posz(double level);

	void light3_posx(double level);
	void light3_posy(double level);
	void light3_posz(double level);

	void light1_ar(int level);
	void light1_ag(int level);
	void light1_ab(int level);
	void light2_ar(int level);
	void light2_ag(int level);
	void light2_ab(int level);
	void light3_ar(int level);
	void light3_ag(int level);
	void light3_ab(int level);

	void light1_dr(int level);
	void light1_dg(int level);
	void light1_db(int level);
	void light2_dr(int level);
	void light2_dg(int level);
	void light2_db(int level);
	void light3_dr(int level);
	void light3_dg(int level);
	void light3_db(int level);
private:
    Ui::FinalModelViewerClass ui;
};
