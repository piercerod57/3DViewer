#pragma once
/**********************************************************
 *
 * FUNCTION Load3DS (obj_type_ptr, char *)
 *
 * This function loads a mesh from a 3ds file.
 * Please note that we are loading only the vertices, polygons and mapping lists.
 * If you need to load meshes with advanced features as for example:
 * multi objects, materials, lights and so on, you must insert other chunk parsers.
 *
 *********************************************************/
#define MAX_VERTICES 80000 // Max number of vertices (for each object)
#define MAX_POLYGONS 80000 // Max number of polygons (for each object)

// Our vertex type
typedef struct {
	float x, y, z;
}vertex_type;

// The polygon (triangle), 3 numbers that aim 3 vertices
typedef struct {
	int a, b, c;
}polygon_type;

// The mapcoord type, 2 texture coordinates for each vertex
typedef struct {
	float u, v;
}mapcoord_type;

// The object type
typedef struct {
	char name[20];

	int vertices_qty;
	int polygons_qty;

	vertex_type vertex[MAX_VERTICES];
	polygon_type polygon[MAX_POLYGONS];
	mapcoord_type mapcoord[MAX_VERTICES];
	int id_texture;
} obj_type, *obj_type_ptr;


extern char Load3DS(obj_type_ptr ogg, const char *filename);