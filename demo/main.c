#include <softberg/softberg.h>
#include <softberg/utils.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>




int main() {
  sb_canvas *canvas = sb_canvas_init(1080, 720);
  if (!canvas) {printf("ERROR: Failed to create canvas: %s\n", strerror(errno)); return 1;}

  sb_mesh mesh = sb_load_obj("models/cube.obj");

  sb_render_mesh(canvas, mesh);

  sb_write_ppm(canvas, "output.ppm");
  sb_canvas_delete(canvas);
  return 0;
}
