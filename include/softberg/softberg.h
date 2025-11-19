#ifndef _SOFTBERG_HEADER
#define _SOFTBERG_HEADER

typedef unsigned int sb_uint;
typedef unsigned char sb_uint8;

typedef struct {
  sb_uint8 r, g, b, a;
} sb_color;

#include "softmath.h"

typedef struct {
  sb_vec3f *vertices;
  sb_vec3i *indices;
  sb_color *colors;
  const sb_uint len;
} sb_mesh;

typedef struct {
  sb_color *data;
  float *depth;
  sb_uint width;
  sb_uint height;
} sb_canvas;

sb_canvas *sb_canvas_init(sb_uint width, sb_uint height);
void sb_canvas_delete(sb_canvas *canvas);

void sb_canvas_fill(sb_canvas *canvas, sb_color color);

void sb_render_triangle(sb_canvas *canvas, sb_triangle3d triangle, sb_transform t, _Bool interpolate_colors);
void sb_render_mesh(sb_canvas *canvas, sb_mesh mesh, sb_transform t, _Bool interpolate_colors);

void sb_mesh_delete(sb_mesh mesh);

#endif //_SOFTBERG_HEADER
