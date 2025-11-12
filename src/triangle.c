#include <softberg/softberg.h>
#include <softberg/softmath.h>

#include "triangle.h"

#include <math.h>






float sign(sb_vec2f p1, sb_vec2f p2, sb_vec2f p3) {
  return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool point_in_triangle(sb_vec2f p, sb_triangle2d tri) {
  sb_vec2f a = tri.v1;
  sb_vec2f b = tri.v2;
  sb_vec2f c = tri.v3;
  float areaABP = sign(a, b, p);
	float areaBCP = sign(b, c, p);
	float areaCAP = sign(c, a, p);
	bool inTri = areaABP >= 0 && areaBCP >= 0 && areaCAP >= 0;

	float totalArea = (areaABP + areaBCP + areaCAP);
	//float invAreaSum = 1 / totalArea;

	return inTri && totalArea > 0;}

sb_vec3f transform_vector(sb_vec3f ihat, sb_vec3f jhat, sb_vec3f khat, sb_vec3f v) {
  return sb_vec3add3(sb_vec3mul1(ihat, v.x), sb_vec3add3(sb_vec3mul1(jhat, v.y), sb_vec3mul1(khat, v.z)));
}

basis_vectors get_basis_vectors(sb_transform t) {
  sb_vec3f ihat_yaw = sb_vec3(cos(t.rotation.x), 0, sin(t.rotation.x));
  sb_vec3f ihat_pitch = sb_vec3(1, 0, 0);
  sb_vec3f ihat_roll = sb_vec3(cos(t.rotation.z), sin(t.rotation.z), 0);

  sb_vec3f jhat_yaw = sb_vec3(0, 1, 0);
  sb_vec3f jhat_pitch = sb_vec3(0, cos(t.rotation.y), -sin(t.rotation.y));
  sb_vec3f jhat_roll = sb_vec3(-sin(t.rotation.z), cos(t.rotation.z), 0);

  sb_vec3f khat_yaw = sb_vec3(-sin(t.rotation.x), 0, cos(t.rotation.x));
  sb_vec3f khat_pitch = sb_vec3(0, sin(t.rotation.y), cos(t.rotation.y));
  sb_vec3f khat_roll = sb_vec3(0, 0, 1);


  sb_vec3f ihat_pitchyaw = transform_vector(ihat_yaw, jhat_yaw, khat_yaw, ihat_pitch);
  sb_vec3f jhat_pitchyaw = transform_vector(ihat_yaw, jhat_yaw, khat_yaw, jhat_pitch);
  sb_vec3f khat_pitchyaw = transform_vector(ihat_yaw, jhat_yaw, khat_yaw, khat_pitch);

  basis_vectors ret = (basis_vectors) {
    transform_vector(ihat_pitchyaw, jhat_pitchyaw, khat_pitchyaw, ihat_roll),
    transform_vector(ihat_pitchyaw, jhat_pitchyaw, khat_pitchyaw, jhat_roll),
    transform_vector(ihat_pitchyaw, jhat_pitchyaw, khat_pitchyaw, khat_roll),
  };
  return ret;
}

sb_triangle2d project_triangle(sb_triangle3d triangle, sb_transform t) {
  basis_vectors bv = get_basis_vectors(t);
  sb_triangle3d transformed = {
    transform_vector(sb_vec3mul1(bv.ihat, t.scale.x), sb_vec3mul1(bv.jhat, t.scale.x), sb_vec3mul1(bv.khat, t.scale.x), triangle.v1),
    transform_vector(sb_vec3mul1(bv.ihat, t.scale.x), sb_vec3mul1(bv.jhat, t.scale.x), sb_vec3mul1(bv.khat, t.scale.x), triangle.v2),
    transform_vector(sb_vec3mul1(bv.ihat, t.scale.x), sb_vec3mul1(bv.jhat, t.scale.x), sb_vec3mul1(bv.khat, t.scale.x), triangle.v3),
    triangle.v1_color,
    triangle.v2_color,
    triangle.v3_color
  };

  return (sb_triangle2d) {
    sb_vec2(transformed.v1.x, transformed.v1.y),
    sb_vec2(transformed.v2.x, transformed.v2.y),
    sb_vec2(transformed.v3.x, transformed.v3.y),
    triangle.v1_color,
    triangle.v2_color,
    triangle.v3_color
  };
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

