
/**
 * CSC 4565/5565 Introduction to Computer Graphics
 * Instructor: Dr. Min Choi
 *
 * The use can modify the properties of the scene by using the right-click
 * context menu.
 *
 * Additionally the following keys manipulate the scene:
 *		Keys j/k rotate light 0
 *		Keys u/i rotate light 1
 *
 * This application is slightly refactored from homework 3 however its functionality
 * now includes environment mapping.
 */
#include <vector>
#include <iostream>
#include <GL/freeglut.h>
#include "Icosahedron.h"
#pragma comment(lib, "glu32.lib") 
 /**
  * To load the C tga image loader it must be considered C code because it is
  * written in C code. Works in Microsoft Visual Studio 2010 Ultimate.
  */
extern "C" {
#include "tga.h"
}

/**
 * Used to declare the constants related to the cube map in OpenGL.
 */
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

 /**
  * Window Specific Constants.
  */
const char* WINDOW_TITLE = "[OpenGL Environment Mapping]";
const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 400;
GLint g_glutWindowIdentifier;

/**
 * Face targets of the cube map.
 */
static GLenum targets[6] = {
  GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT,
  GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT,
  GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT,
  GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT,
  GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT,
  GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT
};

/**
 * Perspective Variables
 */
bool ortho = false;
const GLdouble FOV = 65.0;
const GLdouble CAM_NEAR = 0.1;
const GLdouble CAM_FAR = 10.0;
const GLdouble ORTHO_LEFT = -2.0f;
const GLdouble ORTHO_RIGHT = 2.0f;
const GLdouble ORTHO_TOP = 2.0f;
const GLdouble ORTHO_BOTTOM = -2.0f;

/**
 * LookAt Vectors - eye, look_at, and up
 */
Vector3f eye = Vector3f(0.0f, 0.0f, 5.0f);
Vector3f look_at = Vector3f(0.0f, 0.0f, 0.0f);
Vector3f up = Vector3f(0.0f, 1.0f, 0.0f);

/**
 * The mouse and rotation variables hold the state of the mouse coordinate
 * differences as it is dragged. The rotation of the displayed icosahedron
 * depends on the dragging movement of the mouse.
 */
int mouse_cur[2] = { 0, 0 };
int mouse_last[2] = { 0, 0 };
float rotation_scale = 1.2f;
float rotation_angle_x = 0.0f;
float rotation_angle_y = 0.0f;

/**
 * TGA Images used for the cubemap.
 */
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

/**
 * Enum that is used to determine which mesh is displayed.
 */
enum MeshType { MESH_TORUS, MESH_ICOSAHEDRON_0, MESH_ICOSAHEDRON_1, MESH_ICOSAHEDRON_2, MESH_ICOSAHEDRON_3, MESH_ICOSAHEDRON_4, MESH_TEAPOT };

MeshType mesh = MESH_TEAPOT;
bool mipmaps = true;
int wrap = GL_REPEAT;
bool smooth = true;
bool cube_map_enabled = true;
int cube_map_mode = GL_NORMAL_MAP_EXT;

/**
 * Light 0 Information
 */
float angle_light_0 = 0.0f;
float light_0_dist = 3.0f;
GLfloat light_0_pos[] = { light_0_dist * cos(angle_light_0), 0.0f, light_0_dist * sin(angle_light_0), 0.0f };
GLfloat light_0_ambient[] = { 0.1f, 0.2f, 0.4f, 1.0f };
GLfloat light_0_diffuse[] = { 0.1f, 0.2f, 0.8f, 1.0f };
GLfloat light_0_specular[] = { 0.0f, 0.0f, 1.0f, 1.0f };

/**
 * Light 1 Information
 */
float angle_light_1 = 9.6f;
float light_1_dist = 3.0f;
GLfloat light_1_pos[] = { light_1_dist * cos(angle_light_1), 0.0f, light_1_dist * sin(angle_light_1), 0.0f };
GLfloat light_1_ambient[] = { 0.4f, 0.2f, 0.1f, 1.0f };
GLfloat light_1_diffuse[] = { 0.8f, 0.2f, 0.1f, 1.0f };
GLfloat light_1_specular[] = { 1.0f, 0.0f, 0.0f, 1.0f };

bool lighting = false;
bool light_0_enabled = true;
bool light_1_enabled = true;

/**
 * Set the parameters of the lights used in the scene.
 */
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

/**
 * Loads a TGA texture to the specified target.
 */
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

/**
 * Updates the texture coordinate generation function.
 */
void g_update_texgen() {
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, cube_map_mode);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, cube_map_mode);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, cube_map_mode);
}

/**
 * Updates the texture wrapping type.
 */
void g_update_wrap() {
	glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_WRAP_T, wrap);
}

/**
 * Constructs a cube map from the six images provided.
 */
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
	g_update_wrap();

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
}

/**
 * Creates the 5 levels of different icosahedron sphere approximations.
 */
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
 * Initializes several OpenGL settings and sets up the initial conditions of
 * the program.
 */
void g_init() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	g_create_lights();
	g_construct_cube_map();
	g_create_Icosahedrons();
}

/**
 * The reshape function registered with glut. Sets up a perspective projection
 * or an orthographic projection.
 */
void g_glutReshapeFunc(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (ortho == true)
		glOrtho(ORTHO_LEFT, ORTHO_RIGHT, ORTHO_BOTTOM, ORTHO_TOP, CAM_NEAR, CAM_FAR);
	else
		gluPerspective(FOV, ((GLfloat)width / (GLfloat)height), CAM_NEAR, CAM_FAR);
	gluLookAt(eye.x(), eye.y(), eye.z(), look_at.x(), look_at.y(), look_at.z(), up.x(), up.y(), up.z());
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glutPostRedisplay();
}

/**
 * Using the defined colors and positions of the lights: light0 and light 1
 * this function draws a large point in the defined light color at the light's
 * position.
 */
void g_draw_lights() {
	if (lighting == false) return;
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
	if (cube_map_enabled)
		glEnable(GL_TEXTURE_CUBE_MAP_EXT);
}

/**
 * The display function registered with glut. This function applies the mouse
 * based rotation and then renders the appropriate model.
 */
void g_glutDisplayFunc() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glRotatef(rotation_angle_x, 0.0f, 1.0f, 0.0f);
	glRotatef(rotation_angle_y, 1.0f, 0.0f, 0.0f);

	switch (mesh) {
	case MESH_ICOSAHEDRON_0: spheres[0].render(smooth); break;
	case MESH_ICOSAHEDRON_1: spheres[1].render(smooth); break;
	case MESH_ICOSAHEDRON_2: spheres[2].render(smooth); break;
	case MESH_ICOSAHEDRON_3: spheres[3].render(smooth); break;
	case MESH_ICOSAHEDRON_4: spheres[4].render(smooth); break;
	case MESH_TORUS: glutSolidTorus(0.6, 1.4, 35, 35); break;
	case MESH_TEAPOT: glutSolidTeapot(1.5); break;
	}

	glPopMatrix();
	g_draw_lights();
	glutSwapBuffers();
}

/**
 * This function dispatches the pressed key events.
 */
void g_glutKeyboardFunc(unsigned char key, int x, int y) {
	if (key == 'j') { angle_light_0 += 0.2f; }
	if (key == 'k') { angle_light_0 -= 0.2f; }
	if (key == 'u') { angle_light_1 += 0.2f; }
	if (key == 'i') { angle_light_1 -= 0.2f; }

	if (key == 'r') {
		cube_map_mode = GL_NORMAL_MAP_EXT;
		g_update_texgen();
	}

	if (key == 't') {
		cube_map_mode = GL_REFLECTION_MAP_EXT;
		g_update_texgen();
	}

	light_0_pos[0] = light_0_dist * cos(angle_light_0);
	light_0_pos[2] = light_0_dist * sin(angle_light_0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_0_pos);

	light_1_pos[0] = light_1_dist * cos(angle_light_1);
	light_1_pos[2] = light_1_dist * sin(angle_light_1);
	glLightfv(GL_LIGHT1, GL_POSITION, light_1_pos);

	glutPostRedisplay();
}

/**
 * When the mouse is dragged, it increments or decrements the angle of rotation
 * used to rotate the icosahedron in the scene.
 */
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

/**
 * The following set of functions are used by the g_menuCommands() function.
 */
void g_toggleIcosahedron(int index) {
	for (int i = 0; i < ICOSAHEDRON_COUNT; i++)
		spheres[i].setVisible(false);
	spheres[index].setVisible(true);
}

void menu_icosahedron_0() {
	mesh = MESH_ICOSAHEDRON_0;
	g_toggleIcosahedron(0);
}

void menu_icosahedron_1() {
	mesh = MESH_ICOSAHEDRON_1;
	g_toggleIcosahedron(1);
}

void menu_icosahedron_2() {
	mesh = MESH_ICOSAHEDRON_2;
	g_toggleIcosahedron(2);
}

void menu_icosahedron_3() {
	mesh = MESH_ICOSAHEDRON_3;
	g_toggleIcosahedron(3);
}

void menu_icosahedron_4() {
	mesh = MESH_ICOSAHEDRON_4;
	g_toggleIcosahedron(4);
}

void menu_enable_SmoothShading() {
	glShadeModel(GL_SMOOTH);
	smooth = true;
}

void menu_enable_FlatShading() {
	glShadeModel(GL_FLAT);
	smooth = false;
}

void menu_toggle_EnvironmentMap() {
	if (cube_map_enabled)
		glDisable(GL_TEXTURE_CUBE_MAP_EXT);
	else
		glEnable(GL_TEXTURE_CUBE_MAP_EXT);
	cube_map_enabled = !cube_map_enabled;
}

void menu_toggle_light_0() {
	if (light_0_enabled)
		glDisable(GL_LIGHT0);
	else
		glEnable(GL_LIGHT0);
	light_0_enabled = !light_0_enabled;
}

void menu_toggle_light_1() {
	if (light_1_enabled)
		glDisable(GL_LIGHT1);
	else
		glEnable(GL_LIGHT1);
	light_1_enabled = !light_1_enabled;
}

void menu_toggle_lighting() {
	if (lighting == false) {
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
	}
	else {
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
		glColor3f(1.0f, 1.0f, 1.0f);
	}
	lighting = !lighting;
}

void menu_enable_texRepeat() {
	wrap = GL_REPEAT;
	g_update_wrap();
}

void menu_enable_texClamp() {
	wrap = GL_CLAMP;
	g_update_wrap();
}

/**
 * This function handles all of the messages generated by the right-click
 * context menu.
 * value - The command identifier.
 */
void g_menuCommands(int value) {
	switch (value) {
	case MESH_TORUS: mesh = MESH_TORUS; break;
	case MESH_TEAPOT: mesh = MESH_TEAPOT; break;
	case '0': menu_icosahedron_0(); break;
	case '1': menu_icosahedron_1(); break;
	case '2': menu_icosahedron_2(); break;
	case '3': menu_icosahedron_3(); break;
	case '4': menu_icosahedron_4(); break;
	case 's': menu_enable_SmoothShading(); break;
	case 'f': menu_enable_FlatShading(); break;
	case 'e': menu_toggle_EnvironmentMap(); break;
	case 'x': menu_toggle_light_0(); break;
	case 'z': menu_toggle_light_1(); break;
	case 'u': menu_enable_texRepeat(); break;
	case 'v': menu_enable_texClamp(); break;
	case 'c': menu_toggle_lighting(); break;
	}
	glutPostRedisplay();
}

/**
 * Helper function that creates the right-click context menu to allow the user
 * to modify the properties of the scene.
 */
void g_createMenu() {
	int subdivision_menu = glutCreateMenu(g_menuCommands);
	glutAddMenuEntry("Subdivision Level 0 (20)", '0');
	glutAddMenuEntry("Subdivision Level 1 (80)", '1');
	glutAddMenuEntry("Subdivision Level 2 (320)", '2');
	glutAddMenuEntry("Subdivision Level 3 (1280)", '3');
	glutAddMenuEntry("Subdivision Level 4 (5120)", '4');

	int mesh_menu = glutCreateMenu(g_menuCommands);
	glutAddMenuEntry("Torus", MESH_TORUS);
	glutAddSubMenu("Icosahedron", subdivision_menu);
	glutAddMenuEntry("Teapot", MESH_TEAPOT);

	int shading_menu = glutCreateMenu(g_menuCommands);
	glutAddMenuEntry("Smooth", 's');
	glutAddMenuEntry("Flat", 'f');

	int environment_menu = glutCreateMenu(g_menuCommands);
	glutAddMenuEntry("Toggle Environment Map", 'e');

	int lighting_menu = glutCreateMenu(g_menuCommands);
	glutAddMenuEntry("Toggle Light (0)", 'x');
	glutAddMenuEntry("Toggle Light (1)", 'z');
	glutAddMenuEntry("Toggle Lighting", 'c');

	int texture_menu = glutCreateMenu(g_menuCommands);
	glutAddMenuEntry("Repeat", 'u');
	glutAddMenuEntry("Clamp", 'v');

	int mainmenu = glutCreateMenu(0);
	glutAddSubMenu("Mesh", mesh_menu);
	glutAddSubMenu("Shading", shading_menu);
	glutAddSubMenu("Environment Mapping", environment_menu);
	glutAddSubMenu("Lighting", lighting_menu);
	glutAddSubMenu("Texture", texture_menu);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

/**
 * The main function of the application. No command line arguments beyond the
 * glut provided commands are supported.
 */
int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	g_glutWindowIdentifier = glutCreateWindow(WINDOW_TITLE);

	glutDisplayFunc(g_glutDisplayFunc);
	glutReshapeFunc(g_glutReshapeFunc);
	glutKeyboardFunc(g_glutKeyboardFunc);
	glutMotionFunc(g_glutMotionFunc);
	glutMouseFunc(g_glutMouseFunc);

	g_init();
	g_createMenu();

	glutMainLoop();
	return 0;
}