#ifndef _SOFTBERG_HEADER
#define _SOFTBERG_HEADER
#include "softmath.h"

typedef unsigned int sb_uint;

typedef struct {
  sb_vec3f *vertices;
  sb_vec3i *indices;
  const sb_uint len;
} sb_mesh;

typedef struct {
  unsigned int r, g, b;
} sb_color;

typedef struct {
  sb_color *data;
  float *depth;
  sb_uint width;
  sb_uint height;
} sb_canvas;

sb_canvas *sb_canvas_init(sb_uint width, sb_uint height);
void sb_canvas_delete(sb_canvas *canvas);

void sb_render_triangle(sb_canvas *canvas, sb_triangle3d triangle);
void sb_render_mesh(sb_canvas *canvas, sb_mesh mesh);

sb_mesh sb_load_obj(const char * restrict path);

#endif //_SOFTBERG_HEADER
