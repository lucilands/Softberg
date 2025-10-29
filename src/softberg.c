#include <softberg/softberg.h>
#include <softberg/softmath.h>

#include <stdlib.h>
#include <errno.h>



sb_canvas_t *sb_canvas_init(sb_uint_t width, sb_uint_t height) {
  sb_canvas_t *ret = malloc(sizeof(sb_canvas_t));
  if (!ret) {errno = ENOMEM; return NULL;}

  ret->data = malloc(width * height * sizeof(sb_color_t));
  if (!ret->data) {errno = ENOMEM; return NULL;}

  ret->width = width;
  ret->height = height;

  return ret;
}

void sb_canvas_delete(sb_canvas_t *canvas) {
  free(canvas->data);
  free(canvas);
}
