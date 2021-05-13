#include <GL/glew.h>//Must be included first

#include "QViewport.h"
#include "OpenGL.h"
#include "Icosahedron.h"
#include <QOpenGLContext>
#include <QMouseEvent>
#include "Vector3f.h"
#include "Shaders.h"


vertex_type crossProduct(vertex_type U, vertex_type V)
{
	vertex_type N;
	Vector3f tempU = Vector3f(U.x, U.y, U.z);
	Vector3f tempV = Vector3f(V.x, V.y, V.z);
	Vector3f tempN = Vector3f::cross(tempU, tempV);
	N.x = tempN[0];
	N.y = tempN[1];
	N.z = tempN[2];
	return N;
}

void setLighting(light_type light)
{
	GLfloat DiffuseLight[] = { light.dlr, light.dlg, light.dlb }; //set DiffuseLight[] to the specified values
	GLfloat AmbientLight[] = { light.alr, light.alg, light.alb }; //set AmbientLight[] to the specified values

	glLightfv(light.id, GL_DIFFUSE, DiffuseLight); //change the light accordingly
	glLightfv(light.id, GL_AMBIENT, AmbientLight); //change the light accordingly

	GLfloat LightPosition[] = { light.lx, light.ly, light.lz, light.lw }; //set the LightPosition to  the specified values

	glLightfv(light.id, GL_POSITION, LightPosition); //change the light accordingly
	glEnable(light.id);
}

/*
Vector3f calculateSurfaceNormal()
{
	Begin Function CalculateSurfaceNormal(Input Triangle) Returns Vector

		Set Vector U to(Triangle.p2 minus Triangle.p1)
		Set Vector V to(Triangle.p3 minus Triangle.p1)

		Set Normal.x to(multiply U.y by V.z) minus(multiply U.z by V.y)
		Set Normal.y to(multiply U.z by V.x) minus(multiply U.x by V.z)
		Set Normal.z to(multiply U.x by V.y) minus(multiply U.y by V.x)

		Returning Normal

	End Function
}*/

QViewport::QViewport(QWidget* parent) : QOpenGLWidget(parent) 
{
	
}

QViewport::~QViewport() {}

void QViewport::initializeGL() 
{
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{//glew didn't initialize properly
		printf("%s", glewGetErrorString(err));
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//Type of shading for the polygons
	glShadeModel(GL_SMOOTH); 

	float width = this->width();
	float height = this->height();

	this->xRot = 0;
	this->yRot = 0;
	this->zRot = 0;
	this->zoom = -75.0;

	this->glProgram = createShaders();
	
	//Viewport transformation
	glViewport(0, 0, width, height);

	//Projection transformation
	glMatrixMode(GL_PROJECTION); // Specifies which matrix stack is the target for matrix operations 
	glLoadIdentity(); // We initialize the projection matrix as identity
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 10.0f, 10000.0f); // We define the "viewing volume"
	
	glClearDepth(1);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST); // We enable the depth test (also called z buffer)
	glEnable(GL_LIGHTING);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Polygon rasterization mode (polygon filled)

	//glEnable(GL_TEXTURE_2D); // This Enable the Texture mapping

	for (int i = 0; i < 3; ++i)
	{
		if (i == 0)
		{
			this->lights[i].enabled = true;
			this->lights[i].id = GL_LIGHT0;
		}
		else if (i == 1)
		{
			this->lights[i].enabled = true;
			this->lights[i].id = GL_LIGHT1;
		}
		else if (i == 2)
		{
			this->lights[i].enabled = true;
			this->lights[i].id = GL_LIGHT2;
		}

		this->lights[i].angle = 0.0; //set the angle of rotation

		//diffuse light color variables
		this->lights[i].dlr = 1.0;
		this->lights[i].dlg = 1.0;
		this->lights[i].dlb = 1.0;

		//ambient light color variables
		this->lights[i].alr = 0.2;
		this->lights[i].alg = 0.1;
		this->lights[i].alb = 1.0;

		//light position variables
		this->lights[i].lx = 0.0;
		this->lights[i].ly = 10.0;
		this->lights[i].lz = (GLfloat)(i + 10);
		this->lights[i].lw = 0.0;
	}
}

void QViewport::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int l_index;
	float s = 2.0f;

	//@NOTE(P): Render Mesh
	glPushMatrix();
	for (int i = 0; i < 3; ++i)
	{
		if (this->lights[i].enabled == true)
		{
			setLighting(this->lights[i]);
		}
	}
	
	glMatrixMode(GL_MODELVIEW); //Modeling transformation
	glLoadIdentity(); //Initialize the model matrix as identity
	glUseProgram(this->glProgram);//Binds Shaders
	glTranslatef(0.0, 0.0, -75.0); //apply translation to transformation

	glRotatef(this->xRot / 16.0, 1.0, 0.0, 0.0); //apply rotation on x
	glRotatef(this->yRot / 16.0, 0.0, 1.0, 0.0); //apply rotation on y
	glRotatef(this->zRot / 16.0, 0.0, 0.0, 1.0); //apply rotation on z
	glScaled(this->zoom, this->zoom, this->zoom);//zoom 

	glBegin(GL_TRIANGLES); //glBegin and glEnd delimit the vertices that define a primitive (in our case triangles)
	for (l_index = 0; l_index < this->mesh.polygons_qty; l_index++)
	{
		//---------------- CALCULATE NORMALS ---------------- 
		vertex_type U; //P2 - P1
		vertex_type V; //P3 - P1
		
		U.x = this->mesh.vertex[this->mesh.polygon[l_index].b].x - this->mesh.vertex[this->mesh.polygon[l_index].a].x;
		U.y = this->mesh.vertex[this->mesh.polygon[l_index].b].y - this->mesh.vertex[this->mesh.polygon[l_index].a].y;
		U.z = this->mesh.vertex[this->mesh.polygon[l_index].b].z - this->mesh.vertex[this->mesh.polygon[l_index].a].z;

		V.x = this->mesh.vertex[this->mesh.polygon[l_index].c].x - this->mesh.vertex[this->mesh.polygon[l_index].a].x;
		V.y = this->mesh.vertex[this->mesh.polygon[l_index].c].y - this->mesh.vertex[this->mesh.polygon[l_index].a].y;
		V.z = this->mesh.vertex[this->mesh.polygon[l_index].c].z - this->mesh.vertex[this->mesh.polygon[l_index].a].z;

		vertex_type N = crossProduct(U, V);

		//----------------- FIRST VERTEX -----------------
		// Texture coordinates of the first vertex

		glTexCoord2f(	this->mesh.mapcoord[this->mesh.polygon[l_index].a].u,
						this->mesh.mapcoord[this->mesh.polygon[l_index].a].v);
		// Coordinates of the first vertex
		glVertex3f(	this->mesh.vertex[this->mesh.polygon[l_index].a].x,
					this->mesh.vertex[this->mesh.polygon[l_index].a].y,
					this->mesh.vertex[this->mesh.polygon[l_index].a].z); //Vertex definition

		glNormal3f(N.x, N.y, N.z);//set normals
		//----------------- SECOND VERTEX -----------------
		// Texture coordinates of the second vertex
		glTexCoord2f(	this->mesh.mapcoord[this->mesh.polygon[l_index].b].u,
						this->mesh.mapcoord[this->mesh.polygon[l_index].b].v);
		// Coordinates of the second vertex
		glVertex3f(	this->mesh.vertex[this->mesh.polygon[l_index].b].x,
					this->mesh.vertex[this->mesh.polygon[l_index].b].y,
					this->mesh.vertex[this->mesh.polygon[l_index].b].z);

		glNormal3f(N.x, N.y, N.z);//set normals
		//----------------- THIRD VERTEX -----------------
		// Texture coordinates of the third vertex
		glTexCoord2f(	this->mesh.mapcoord[this->mesh.polygon[l_index].c].u,
						this->mesh.mapcoord[this->mesh.polygon[l_index].c].v);
		// Coordinates of the Third vertex
		glVertex3f(		this->mesh.vertex[this->mesh.polygon[l_index].c].x,
						this->mesh.vertex[this->mesh.polygon[l_index].c].y,
						this->mesh.vertex[this->mesh.polygon[l_index].c].z);
		
		glNormal3f(N.x, N.y, N.z);//set normals
	}
	glUseProgram(0);//Unbinds shaders
	glEnd();
	glPopMatrix();
}

void QViewport::resizeGL(int width, int height) 
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0, ((GLfloat)width / (GLfloat)height), 0.01, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	
}


//Mouse Movement functions
void qNormalizeAngle(int* angle)
{
	while (*angle < 0)
	{
		*angle += 360 * 16;
	}
	while (*angle > 360 * 16)
	{
		*angle -= 360 * 16;
	}
}

void QViewport::setXRotation(int angle)
{
	qNormalizeAngle(&angle);
	if (angle != xRot) 
	{
		xRot = angle;
		emit xRotationChanged(angle);
		update();
	}
}

void QViewport::setYRotation(int angle)
{
	qNormalizeAngle(&angle);
	if (angle != yRot) 
	{
		yRot = angle;
		emit yRotationChanged(angle);
		update();
	}
}

void QViewport::setZRotation(int angle)
{
	qNormalizeAngle(&angle);
	if (angle != zRot) 
	{
		zRot = angle;
		emit zRotationChanged(angle);
		update();
	}
}

void QViewport::mousePressEvent(QMouseEvent *event)
{
	this->lastPos = event->pos();
}

void QViewport::mouseMoveEvent(QMouseEvent *event)
{
	int dx = event->x() - lastPos.x();
	int dy = event->y() - lastPos.y();

	if (event->buttons() & Qt::LeftButton)
	{
		setXRotation(this->xRot + 8 * dy);
		setYRotation(this->yRot + 8 * dx);
	}
	else if (event->buttons() & Qt::RightButton)
	{
		setXRotation(xRot + 8 * dy);
		setZRotation(zRot + 8 * dx);
	}
	
	lastPos = event->pos();
}

void QViewport::wheelEvent(QWheelEvent *event)
{
	QPoint numDegrees = event->angleDelta();
	if (numDegrees.y() < 0)
	{
		zoom = zoom / 1.1;
	}
	if (numDegrees.y() > 0) 
	{
		zoom = zoom * 1.1;
	}
	update();
}