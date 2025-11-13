#include <softberg/softberg.h>
#include <softberg/softmath.h>

#include "triangle.h"

#include <float.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


void sb_render_triangle(sb_canvas *canvas, sb_triangle3d triangle, sb_transform t, bool interpolate_colors) {
  sb_triangle3d projected_triangle = project_triangle(triangle, t);
  sb_triangle2d projected_triangle2d = (sb_triangle2d) {
    sb_vec3xy(projected_triangle.v1),
    sb_vec3xy(projected_triangle.v2),
    sb_vec3xy(projected_triangle.v3),

    projected_triangle.v1_color,
    projected_triangle.v2_color,
    projected_triangle.v3_color,
  };
  //sb_triangle2d projected_triangle = project_triangle(triangle, 10, 10, t);
  center_triangle(canvas, &projected_triangle2d);

  bounding_box bounding_box = calculate_bb(canvas, projected_triangle2d);

  sb_color pix_col = triangle.v1_color;
  float depth = 0.0f;

  for (sb_uint x = bounding_box.topleft.x; x < bounding_box.bottomright.x; x++) {
    for (sb_uint y = bounding_box.topleft.y; y < bounding_box.bottomright.y; y++) {
      int index = x + canvas->width * y;
      //depth = depth_at_point(sb_vec2(x, y), projected_triangle);
      depth = projected_triangle.v1.z;
      if (interpolate_colors) pix_col = interpolate_color(sb_vec2(x, y), projected_triangle2d);
      if (point_in_triangle(sb_vec2(x, y), projected_triangle2d)) {
        canvas->depth[index] = depth;
        canvas->data[index] = (sb_color) {255, 255, 255};
        canvas->data[index] = pix_col;
      }
    }
  }
}

void sb_render_mesh(sb_canvas *canvas, sb_mesh mesh, sb_transform t, bool interpolate_colors) {
  for (sb_uint i = 0; i < mesh.len; i++) {
    sb_vec3i idx = mesh.indices[i];
    sb_triangle3d tri = {mesh.vertices[idx.x], mesh.vertices[idx.y], mesh.vertices[idx.z], mesh.colors[idx.x], mesh.colors[idx.y], mesh.colors[idx.z]};
    sb_render_triangle(canvas, tri, t, interpolate_colors);
  }
}

sb_canvas *sb_canvas_init(sb_uint width, sb_uint height) {
  sb_canvas *ret = malloc(sizeof(sb_canvas));
  if (!ret) {errno = ENOMEM; return NULL;}

  ret->data = malloc(width * height * sizeof(sb_color));
  if (!ret->data) {errno = ENOMEM; return NULL;}

  ret->depth = malloc(width * height * sizeof(float));
  if (!ret->depth) {errno = ENOMEM; return NULL;}

  ret->width = width;
  ret->height = height;

  srand(time(NULL));

  return ret;
}

void sb_canvas_delete(sb_canvas *canvas) {
  free(canvas->data);
  free(canvas->depth);
  free(canvas);
}

void sb_canvas_fill(sb_canvas *canvas, sb_color color) {
  memset(canvas->depth, 0, (canvas->width * canvas->height) * sizeof(float));
  for (sb_uint i = 0; i < canvas->width * canvas->height; i++) {
    canvas->data[i] = color;
  }
}

void sb_mesh_delete(sb_mesh mesh) {
  free(mesh.vertices);
  free(mesh.indices);
  free(mesh.colors);
}
