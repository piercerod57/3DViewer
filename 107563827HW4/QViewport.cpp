#include "QViewport.h"
#include "OpenGL.h"
#include "Icosahedron.h"
#include <QOpenGLContext>


//extern "C" {
//#include "tga.h"
//}

/**

#ifndef GL_EXT_texture_cube_map
# define GL_NORMAL_MAP_EXT                   0x8511
# define GL_REFLECTION_MAP_EXT               0x8512
# define GL_TEXTURE_CUBE_MAP_EXT             0x8513
# define GL_TEXTURE_BINDING_CUBE_MAP_EXT     0x8514
# define GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT  0x8515
# define GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT  0x8516
# define GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT  0x8517
# define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT  0x8518
# define GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT  0x8519
# define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT  0x851A
# define GL_PROXY_TEXTURE_CUBE_MAP_EXT       0x851B
# define GL_MAX_CUBE_MAP_TEXTURE_SIZE_EXT    0x851C
#endif


static GLenum targets[6] = {
  GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT,
  GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT,
  GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT,
  GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT,
  GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT,
  GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT
};


bool ortho = false;
const GLdouble FOV = 65.0;
const GLdouble CAM_NEAR = 0.1;
const GLdouble CAM_FAR = 10.0;
const GLdouble ORTHO_LEFT = -2.0f;
const GLdouble ORTHO_RIGHT = 2.0f;
const GLdouble ORTHO_TOP = 2.0f;
const GLdouble ORTHO_BOTTOM = -2.0f;

Vector3f eye = Vector3f(0.0f, 0.0f, 5.0f);
Vector3f look_at = Vector3f(0.0f, 0.0f, 0.0f);
Vector3f up = Vector3f(0.0f, 1.0f, 0.0f);
int mouse_cur[2] = { 0, 0 };
int mouse_last[2] = { 0, 0 };
float rotation_scale = 1.2f;
float rotation_angle_x = 0.0f;
float rotation_angle_y = 0.0f;

char* cube_images[6] =
{
  "cm_left.tga",
  "cm_right.tga",
  "cm_top.tga",
  "cm_bottom.tga",
  "cm_back.tga",
  "cm_front.tga",
};
const int ICOSAHEDRON_COUNT = 5;
const int ICOSAHEDRON_SHININESS = 24;
Icosahedron spheres[ICOSAHEDRON_COUNT];

bool mipmaps = true;

int cube_map_mode = GL_NORMAL_MAP_EXT;


float angle_light_0 = 0.0f;
float light_0_dist = 3.0f;
GLfloat light_0_pos[] = { light_0_dist * cos(angle_light_0), 0.0f, light_0_dist * sin(angle_light_0), 0.0f };
GLfloat light_0_ambient[] = { 0.1f, 0.2f, 0.4f, 1.0f };
GLfloat light_0_diffuse[] = { 0.1f, 0.2f, 0.8f, 1.0f };
GLfloat light_0_specular[] = { 0.0f, 0.0f, 1.0f, 1.0f };


float angle_light_1 = 9.6f;
float light_1_dist = 3.0f;
GLfloat light_1_pos[] = { light_1_dist * cos(angle_light_1), 0.0f, light_1_dist * sin(angle_light_1), 0.0f };
GLfloat light_1_ambient[] = { 0.4f, 0.2f, 0.1f, 1.0f };
GLfloat light_1_specular[] = { 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat light_1_diffuse[] = { 0.8f, 0.2f, 0.1f, 1.0f };



void g_create_lights() {
	glLightfv(GL_LIGHT0, GL_POSITION, light_0_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_0_specular);

	glLightfv(GL_LIGHT1, GL_POSITION, light_1_pos);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_1_specular);
}


/* Loads a TGA texture to the specified target.

void g_load_texture(GLenum target, char* filename) {
	FILE* file;
	gliGenericImage* image;

	file = fopen(filename, "rb");
	if (file == NULL) exit(1);

	image = gliReadTGA(file, filename);
	fclose(file);

	if (mipmaps == true)
		gluBuild2DMipmaps(target, image->components, image->width, image->height, image->format, GL_UNSIGNED_BYTE, image->pixels);
	else
		glTexImage2D(target, 0, image->components, image->width, image->height, 0, image->format, GL_UNSIGNED_BYTE, image->pixels);
}


void g_update_texgen() {
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, cube_map_mode);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, cube_map_mode);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, cube_map_mode);
}



void g_construct_cube_map() {
	for (int i = 0; i < 6; i++)
		g_load_texture(targets[i], cube_images[i]);

	if (mipmaps == true) {
		glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {
		glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	glEnable(GL_TEXTURE_CUBE_MAP_EXT);

	g_update_texgen();
	//g_update_wrap();

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
} 


void g_create_Icosahedrons() {
	for (int i = 0; i < ICOSAHEDRON_COUNT; i++) {
		spheres[i] = Icosahedron();
		spheres[i].subdivide(i);
		spheres[i].setVisible(false);
		spheres[i].setAmbient(0.4f, 0.4f, 0.4f, 1.0f);
		spheres[i].setDiffuse(0.6f, 0.6f, 0.6f, 1.0f);
		spheres[i].setSpecular(0.8f, 0.8f, 0.8f, 1.0f);
		spheres[i].setShininess(ICOSAHEDRON_SHININESS);
	}
}

/**
 * When the mouse is dragged, it increments or decrements the angle of rotation
 * used to rotate the icosahedron in the scene.

void g_glutMotionFunc(int x, int y) {
	mouse_cur[0] = x;
	mouse_cur[1] = y;

	int x_diff = mouse_last[0] - mouse_cur[0];
	int y_diff = mouse_last[1] - mouse_cur[1];

	x_diff *= -1;

	rotation_angle_x += rotation_scale * (float)x_diff;
	rotation_angle_y += rotation_scale * (float)y_diff;

	mouse_last[0] = mouse_cur[0];
	mouse_last[1] = mouse_cur[1];
	glutPostRedisplay();
}

void g_glutMouseFunc(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		mouse_cur[0] = x;
		mouse_cur[1] = y;
		mouse_last[0] = mouse_cur[0];
		mouse_last[1] = mouse_cur[1];
	}
} 

void QViewport::g_update_wrap() {
	glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_WRAP_S, this->wrap);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_WRAP_T, this->wrap);
}

void QViewport::g_draw_lights() {
	if (this->lighting == false) return;
	glDisable(GL_TEXTURE_CUBE_MAP_EXT);
	glDisable(GL_LIGHTING);
	glPointSize(6.0f);
	glBegin(GL_POINTS);
	glColor3f(light_0_diffuse[0], light_0_diffuse[1], light_0_diffuse[2]);
	glVertex3f(light_0_pos[0], light_0_pos[1], light_0_pos[2]);
	glColor3f(light_1_diffuse[0], light_1_diffuse[1], light_1_diffuse[2]);
	glVertex3f(light_1_pos[0], light_1_pos[1], light_1_pos[2]);
	glEnd();
	glEnable(GL_LIGHTING);
	if (this->cube_map_enabled)
		glEnable(GL_TEXTURE_CUBE_MAP_EXT);
}
*/

QViewport::QViewport(QWidget* parent) : QOpenGLWidget(parent) {
	
}

QViewport::~QViewport() {}

void QViewport::initializeGL() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glShadeModel(GL_SMOOTH); // Type of shading for the polygons

	float width = this->width();
	float height = this->height();

	// Viewport transformation
	glViewport(0, 0, width, height);

	// Projection transformation
	glMatrixMode(GL_PROJECTION); // Specifies which matrix stack is the target for matrix operations 
	glLoadIdentity(); // We initialize the projection matrix as identity
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 10.0f, 10000.0f); // We define the "viewing volume"

	glEnable(GL_DEPTH_TEST); // We enable the depth test (also called z buffer)
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Polygon rasterization mode (polygon filled)

	glEnable(GL_TEXTURE_2D); // This Enable the Texture mapping

	//Load3DS(&this->mesh, "P:\MCBACK\CSCI\graphics\107563827Final\107563827Final\107563827HW4\spaceship.3ds");

	//light test
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };

	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };

	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	//---

	this->mesh.id_texture = 0;
}

void QViewport::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	int l_index;
	float s = 2.0f;
	
	glPushMatrix();
		gluLookAt(0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		
		glBegin(GL_LINES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, 0.0f);
		glVertex3f(s, 0.0f, 0.0f);

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, s, 0.0f);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, s);

		
		glEnd();
	glPopMatrix();

	glPushMatrix();
		glMatrixMode(GL_MODELVIEW); // Modeling transformation
		glLoadIdentity(); // Initialize the model matrix as identity

		//glTranslatef(0.0, 0.0, 0.0); // We move the object forward (the model matrix is multiplied by the translation matrix)

		glBindTexture(GL_TEXTURE_2D, this->mesh.id_texture); // We set the active texture 

		glBegin(GL_TRIANGLES); // glBegin and glEnd delimit the vertices that define a primitive (in our case triangles)
		for (l_index = 0; l_index < this->mesh.polygons_qty; l_index++)
		{
			//----------------- FIRST VERTEX -----------------
			// Texture coordinates of the first vertex
			
			glTexCoord2f(	this->mesh.mapcoord[	this->mesh.polygon[l_index].a	].u,
							this->mesh.mapcoord[	this->mesh.polygon[l_index].a	].v);
			// Coordinates of the first vertex
			glVertex3f(		this->mesh.vertex[	this->mesh.polygon[l_index].a	].x,
							this->mesh.vertex[	this->mesh.polygon[l_index].a	].y,
							this->mesh.vertex[	this->mesh.polygon[l_index].a	].z); //Vertex definition

			//----------------- SECOND VERTEX -----------------
			// Texture coordinates of the second vertex
			glTexCoord2f(this->mesh.mapcoord[this->mesh.polygon[l_index].b].u,
			this->mesh.mapcoord[this->mesh.polygon[l_index].b].v);
			// Coordinates of the second vertex
			glVertex3f(this->mesh.vertex[this->mesh.polygon[l_index].b].x,
			this->mesh.vertex[this->mesh.polygon[l_index].b].y,
			this->mesh.vertex[this->mesh.polygon[l_index].b].z);

			//----------------- THIRD VERTEX -----------------
			// Texture coordinates of the third vertex
			glTexCoord2f(this->mesh.mapcoord[this->mesh.polygon[l_index].c].u,
				this->mesh.mapcoord[this->mesh.polygon[l_index].c].v);
			// Coordinates of the Third vertex
			glVertex3f(this->mesh.vertex[this->mesh.polygon[l_index].c].x,
			this->mesh.vertex[this->mesh.polygon[l_index].c].y,
			this->mesh.vertex[this->mesh.polygon[l_index].c].z);
		}
		glEnd();
	glPopMatrix();
}


void QViewport::resizeGL(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0, ((GLfloat)width / (GLfloat)height), 0.01, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	
}

