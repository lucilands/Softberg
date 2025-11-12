#include <softberg/softberg.h>
#include <softberg/softmath.h>

#include "triangle.h"

#include <math.h>






float sign(sb_vec2f p1, sb_vec2f p2, sb_vec2f p3) {
  return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool point_in_triangle(sb_vec2f pt, sb_triangle2d tri) {
  sb_vec2f v1 = tri.v1;
  sb_vec2f v2 = tri.v2;
  sb_vec2f v3 = tri.v3;
  float d1, d2, d3;
  bool has_neg, has_pos;

  d1 = sign(pt, v1, v2);
  d2 = sign(pt, v2, v3);
  d3 = sign(pt, v3, v1);

  has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
  has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

  return !(has_neg && has_pos);
}


sb_triangle2d project_triangle(sb_triangle3d triangle) {
  return (sb_triangle2d) {sb_vec2(triangle.v1.x * 100.0f, triangle.v1.y * 100.0f), sb_vec2(triangle.v2.x * 100.0f, triangle.v2.y * 100.0f), sb_vec2(triangle.v3.x * 100.0f, triangle.v3.y * 100.0f)};
}

// NOTE: I stole this function from Sebastian Lague
bounding_box calculate_bb(sb_canvas *canvas, sb_triangle2d triangle) {
  sb_vec2f a = triangle.v1;
  sb_vec2f b = triangle.v2;
  sb_vec2f c = triangle.v3;

  float minX = fmin(fmin(a.x, b.x), c.x);
	float minY = fmin(fmin(a.y, b.y), c.y);
	float maxX = fmax(fmax(a.x, b.x), c.x);
	float maxY = fmax(fmax(a.y, b.y), c.y);

  int blockStartX = clamp((int)(minX), 0, (int)canvas->width - 1);
	int blockStartY = clamp((int)(minY), 0, (int)canvas->height - 1);
	int blockEndX = clamp((int)ceil(maxX), 0, (int)canvas->width - 1);
	int blockEndY = clamp((int)ceil(maxY), 0, (int)canvas->height - 1);

  return (bounding_box) {sb_vec2(blockStartX, blockStartY), sb_vec2(blockEndX, blockEndY)};
}

void center_triangle(sb_canvas *canvas, sb_triangle2d *triangle) {
  triangle->v1.x += canvas->width / 2;
  triangle->v2.x += canvas->width / 2;
  triangle->v3.x += canvas->width / 2;

  triangle->v1.y += canvas->height / 2;
  triangle->v2.y += canvas->height / 2;
  triangle->v3.y += canvas->height / 2;
}

