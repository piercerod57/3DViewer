#pragma once

#include <QOpenGLWidget>
#include "Icosahedron.h"
#include "LoadModel.h"


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

	
	obj_type mesh;
	obj_type_ptr ogg = &mesh;

	double xRot;
	double yRot;
	double zRot;
	double zoom;
	QPoint lastPos;

	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
public slots:
	void setXRotation(int angle);
	void setYRotation(int angle);
	void setZRotation(int angle);

signals:
	void xRotationChanged(int angle);
	void yRotationChanged(int angle);
	void zRotationChanged(int angle);
};
