#include <softberg/softberg.h>
#include <softberg/utils.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/stat.h>
#include <sys/types.h>

int mkdir_exist(const char *path) {
#ifdef _WIN32
    int result = _mkdir(path);
#else
    int result = mkdir(path, 0755);
#endif
    if (result == 0) return 0; // created
#ifdef _WIN32
    if (errno == EEXIST) return 0;
#else
    if (errno == EEXIST) return 0;
#endif
    return -1; // error
}


sb_transform transform = {
  .position = {1.0f, 1.0f, 1.0f},
  .rotation = {0.0f, 0.0f, 0.0f},
  .scale = {100.0f, 100.0f, 100.0f}
};


int main() {
  sb_canvas *canvas = sb_canvas_init(1080, 720);
  if (!canvas) {printf("ERROR: Failed to create canvas: %s\n", strerror(errno)); return 1;}

  sb_mesh mesh = sb_load_obj("models/cube.obj");

  mkdir_exist("output");
  char name[256];
  for (unsigned int i = 0; i < 63; i++) {
    sb_canvas_fill(canvas, (sb_color) {0, 0, 0});

    snprintf(name, 256, "output/frame_%02i.ppm", i);
    printf("Rendering frame %02i to %s\n", i, name);

    sb_render_mesh(canvas, mesh, transform);
    sb_write_ppm(canvas, name);
    transform.rotation.y += 0.1;
    transform.rotation.x += 0.1;
  }
  //sb_render_triangle(canvas, triangle, transform);

  sb_canvas_delete(canvas);
  return 0;
}
