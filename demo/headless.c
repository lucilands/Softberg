#include <softberg/softberg.h>
#include <softberg/utils.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>


sb_transform transform = {
	.position = {1.0f, 1.0f, 1.0f},
	.rotation = {0.0f, 0.0f, 0.0f},
	.scale = {100.0f, 100.0f, 100.0f}
};

const sb_uint WIDTH = 1080;
const sb_uint HEIGHT = 720;


int main() {
	sb_canvas *canvas = sb_canvas_init(WIDTH, HEIGHT);
	if (!canvas) {printf("ERROR: Failed to create canvas: %s\n", strerror(errno)); return 1;}

	sb_mesh mesh = sb_load_obj("models/suzanne.obj"); 
	sb_canvas_fill(canvas, (sb_color) {0, 0, 0, 255});
	sb_render_mesh(canvas, mesh, transform, 1);

	sb_write_ppm(canvas, "snapshot_color.ppm");
	sb_write_depth_ppm(canvas, "snapshot_depth.ppm");

	sb_canvas_delete(canvas);
	sb_mesh_delete(mesh);
}
