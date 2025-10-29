#include <softberg/softberg.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>



int main() {
  sb_canvas_t *canvas = sb_canvas_init(1080, 720);
  if (!canvas) {printf("ERROR: Failed to create canvas: %s\n", strerror(errno)); return 1;}

  sb_canvas_delete(canvas);
  return 0;
}
