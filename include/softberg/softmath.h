#ifndef _SOFTMATH_HEADER
#define _SOFTMATH_HEADER

typedef struct {
  float x, y;
} sb_vec2f;

typedef struct {
  float x, y, z;
} sb_vec3f;

typedef struct {
  int x, y, z;
} sb_vec3i;

typedef struct {
  sb_vec3f v1, v2, v3;
} sb_triangle3d;

typedef struct {
  sb_vec2f v1, v2, v3;
} sb_triangle2d;

typedef struct {
  sb_vec3f position;
} sb_transform;

#define sb_vec2(x, y) (sb_vec2f) {(x), (y)}
#define sb_vec3(x, y, z) (sb_vec3f) {(x), (y), (z)}

#endif //_SOFTMATH_HEADER
