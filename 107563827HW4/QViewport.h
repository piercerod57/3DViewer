#pragma once

#include <QOpenGLWidget>
#include "LoadModel.h"
#include "Vector3f.h"

enum SHADER_ENUM { SHADER_NORMAL, SHADER_PIXEL, SHADER_PHONG, SHADER_CUSTOM1, SHADER_CUSTOM2 };


typedef struct 
{
	bool enabled;
	
	GLenum id;

	GLfloat angle = 0.0; //set the angle of rotation

	//ambient light color variables
	GLfloat alr;
	GLfloat alg;
	GLfloat alb;

	//diffuse light color variables
	GLfloat dlr;
	GLfloat dlg;
	GLfloat dlb;

	//light position variables
	GLfloat lx;
	GLfloat ly;
	GLfloat lz;
	GLfloat lw;
}light_type;

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
	void initBufferObject();

	GLuint pVBO, nVBO, EBO, VAO;

	obj_type mesh;
	obj_type_ptr ogg = &mesh;

	light_type lights[3];

	double xRot;
	double yRot;
	double zRot;
	double zoom;
	QPoint lastPos;

	unsigned int glProgram;

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
