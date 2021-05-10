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