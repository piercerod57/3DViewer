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

	obj_type_ptr ogg = &this->ui.viewport->mesh;
	// copy QString to char*
	

	
	Load3DS(ogg, filename.toStdString().c_str());
}