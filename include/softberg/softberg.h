#ifndef _SOFTBERG_HEADER
#define _SOFTBERG_HEADER

typedef unsigned int sb_uint_t;

typedef struct {
  unsigned int r, g, b;
} sb_color_t;

typedef struct {
  sb_color_t *data;
  sb_uint_t width;
  sb_uint_t height;
} sb_canvas_t;

sb_canvas_t *sb_canvas_init(sb_uint_t width, sb_uint_t height);
void sb_canvas_delete(sb_canvas_t *canvas);

#endif //_SOFTBERG_HEADER
