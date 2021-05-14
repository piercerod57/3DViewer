#include "FinalModelViewer.h"
#include <iostream>
#include <QFileDialog>
#include "LoadModel.h"
#include <string>

FinalModelViewer::FinalModelViewer(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}



void FinalModelViewer::loadFromFile() {
	QString filename = QFileDialog::getOpenFileName(
		this,
		"Open Document",
		QDir::currentPath(),
		"3ds files (*.3ds)");

	if (filename.isNull())
	{
		std::cout << "ERROR: Filename null." << std::endl;
	}
	else
	{
		std::cout << filename.toStdString().c_str() << std::endl;
	}

	
	// copy QString to char*
	char* filename_dup = strdup(filename.toStdString().c_str());
	
	int var = Load3DS(this->ui.viewport->ogg, filename_dup);
	if (var == 1)
	{
		std::cout << "SUCCESS: File read." << std::endl;
	}
	else if (var == 0)
	{
		std::cout << "Error: File not found." << std::endl;
	}

}

void FinalModelViewer::light1_enabled(int status)
{
	if (status == Qt::Checked)
	{
		this->ui.viewport->lights[0].enabled = true;
	}
	else if (status == Qt::Unchecked)
	{
		this->ui.viewport->lights[0].enabled = false;
	}
	update();
}

void FinalModelViewer::light2_enabled(int status)
{
	if (status == Qt::Checked)
	{
		this->ui.viewport->lights[1].enabled = true;
	}
	else if (status == Qt::Unchecked)
	{
		this->ui.viewport->lights[1].enabled = false;
	}
	update();
}

void FinalModelViewer::light3_enabled(int status)
{
	if (status == Qt::Checked)
	{
		this->ui.viewport->lights[2].enabled = true;
	}
	else if (status == Qt::Unchecked)
	{
		this->ui.viewport->lights[2].enabled = false;
	}
	update();
}
//light1
void FinalModelViewer::light1_posx(double level)
{
	this->ui.viewport->lights[0].lx = level;
	update();
}

void FinalModelViewer::light1_posy(double level)
{
	this->ui.viewport->lights[0].ly = level;
	update();
}

void FinalModelViewer::light1_posz(double level)
{
	this->ui.viewport->lights[0].lz = level;
	update();
}
//light2
void FinalModelViewer::light2_posx(double level)
{
	this->ui.viewport->lights[1].lx = level;
	update();
}

void FinalModelViewer::light2_posy(double level)
{
	this->ui.viewport->lights[1].ly = level;
	update();
}

void FinalModelViewer::light2_posz(double level)
{
	this->ui.viewport->lights[1].lz = level;
	update();
}
//light3
void FinalModelViewer::light3_posx(double level)
{
	this->ui.viewport->lights[2].lx = level;
	update();
}

void FinalModelViewer::light3_posy(double level)
{
	this->ui.viewport->lights[2].ly = level;
	update();
}

void FinalModelViewer::light3_posz(double level)
{
	this->ui.viewport->lights[2].lz = level;
	update();
}
//-------------------------------------------------
//light1
double intToDouble(int level)
{
	double newValue;
	double temp = level;
	newValue = temp * .01;
	return newValue;
}

void FinalModelViewer::light1_ar(int level)
{
	this->ui.viewport->lights[0].alr = intToDouble(level);
	update();
}

void FinalModelViewer::light1_ag(int level)
{
	this->ui.viewport->lights[0].alg = intToDouble(level);
	update();
}

void FinalModelViewer::light1_ab(int level)
{
	this->ui.viewport->lights[0].alb = intToDouble(level);
	update();
}
//light2
void FinalModelViewer::light2_ar(int level)
{
	this->ui.viewport->lights[1].alr = intToDouble(level);
	update();
}

void FinalModelViewer::light2_ag(int level)
{
	this->ui.viewport->lights[1].alg = intToDouble(level);
	update();
}

void FinalModelViewer::light2_ab(int level)
{
	this->ui.viewport->lights[1].alb = intToDouble(level);
	update();
}
//light3
void FinalModelViewer::light3_ar(int level)
{
	this->ui.viewport->lights[2].alr = intToDouble(level);
	update();
}

void FinalModelViewer::light3_ag(int level)
{
	this->ui.viewport->lights[2].alg = intToDouble(level);
	update();
}

void FinalModelViewer::light3_ab(int level)
{
	this->ui.viewport->lights[2].alb = intToDouble(level);
	update();
}
//-------------------------------------------------

//-------------------------------------------------
//light1
void FinalModelViewer::light1_dr(int level)
{
	this->ui.viewport->lights[0].dlr = intToDouble(level);
	update();
}

void FinalModelViewer::light1_dg(int level)
{
	this->ui.viewport->lights[0].dlg = intToDouble(level);
	update();
}

void FinalModelViewer::light1_db(int level)
{
	this->ui.viewport->lights[0].dlb = intToDouble(level);
	update();
}
//light2
void FinalModelViewer::light2_dr(int level)
{
	this->ui.viewport->lights[1].dlr = intToDouble(level);
	update();
}

void FinalModelViewer::light2_dg(int level)
{
	this->ui.viewport->lights[1].dlg = intToDouble(level);
	update();
}

void FinalModelViewer::light2_db(int level)
{
	this->ui.viewport->lights[1].dlb = intToDouble(level);
	update();
}
//light3
void FinalModelViewer::light3_dr(int level)
{
	this->ui.viewport->lights[2].dlr = intToDouble(level);
	update();
}

void FinalModelViewer::light3_dg(int level)
{
	this->ui.viewport->lights[2].dlg = intToDouble(level);
	update();
}

void FinalModelViewer::light3_db(int level)
{
	this->ui.viewport->lights[2].dlb = intToDouble(level);
	update();
}
//-------------------------------------------------