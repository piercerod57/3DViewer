#pragma once

#include <QOpenGLWidget>
/*
*	QT Viewport Class
*/
class QViewport : public QOpenGLWidget {
	Q_OBJECT

public:
	QViewport(QWidget* parent);
	virtual ~QViewport();

	/* Main 3 functions to define for our own custom functionality */
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);

	//void loadMesh();

	/* Direct access to these, improve as you would like */
	//obj_type object;
	
};