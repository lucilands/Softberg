#ifndef __SHARED_H
#define __SHARED_H
#include <softberg/softmath.h>


#define clamp(value, min_val, max_val) ((value) < (min_val) ? (min_val) : ((value) > (max_val) ? (max_val) : (value)))

typedef _Bool bool;
typedef struct {
  sb_vec2f topleft;
  sb_vec2f bottomright;
} bounding_box;

typedef struct {
  sb_vec3f ihat, jhat, khat;
} basis_vectors;

sb_triangle2d project_triangle(sb_triangle3d triangle, sb_transform t);
void center_triangle(sb_canvas *canvas, sb_triangle2d *triangle);
bounding_box calculate_bb(sb_canvas *canvas, sb_triangle2d triangle);
bool point_in_triangle(sb_vec2f pt, sb_triangle2d tri);

#endif //__SHARED_H
