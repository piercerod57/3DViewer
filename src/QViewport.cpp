#include "QViewport.h"
#include "OpenGL.h"
#include "Icosahedron.h"
#include <QOpenGLContext>


QViewport::~QViewport() {}

void QViewport::initializeGL() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };

	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };

	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
}

void QViewport::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();
	gluLookAt(0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	float s = 1.5f;
	glLineWidth(4.0f);

	// Draw axes lines (x, y, z) as (r, g, b)
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(s, 0.0f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, s, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, s);
	glEnd();

	// If you want to change the color of the light use the following code
	// and comment out the icos.setDiffuse
	//GLfloat light_diff[] = { this->mat_r, this->mat_g, this->mat_b, 1.0 };
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diff);

	glEnable(GL_LIGHTING);
	


	glPopMatrix();
}

void QViewport::resizeGL(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0, float(width) / float(height), 0.01, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}

