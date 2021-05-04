#pragma once

#include <QOpenGLWidget>
#include "Icosahedron.h"


/**
 * Enum that is used to determine which mesh is displayed.
 */
enum MeshType { MESH_TORUS, MESH_ICOSAHEDRON_0, MESH_ICOSAHEDRON_1, MESH_ICOSAHEDRON_2, MESH_ICOSAHEDRON_3, MESH_ICOSAHEDRON_4, MESH_TEAPOT };


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


	/* Direct access to these, improve as you would like */
	MeshType mesh;
	int subdivision;
	bool cube_map_enabled;
	bool lighting;
	bool light_0_enabled;
	bool light_1_enabled;
	bool smooth;
	int wrap;
};
