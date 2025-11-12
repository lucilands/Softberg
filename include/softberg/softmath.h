#ifndef _SOFTMATH_HEADER
#define _SOFTMATH_HEADER

#include "softberg.h"

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
  sb_color v1_color, v2_color, v3_color;
} sb_triangle3d;

typedef struct {
  sb_vec2f v1, v2, v3;
  sb_color v1_color, v2_color, v3_color;
} sb_triangle2d;

typedef struct {
  sb_vec3f position;
  sb_vec3f rotation;
  sb_vec3f scale;
} sb_transform;

#define sb_vec2(x, y) (sb_vec2f) {(x), (y)}
#define sb_vec3(x, y, z) (sb_vec3f) {(x), (y), (z)}

#define sb_vec3xy(vec) (sb_vec2f) {(vec.x), (vec.y)}

#define sb_vec3mul3(a, b) (sb_vec3f) {(a.x) * (b.x), (a.y) * (b.y), (a.z) * (b.z)}
#define sb_vec3mul1(a, b) (sb_vec3f) {(a.x) * (b), (a.y) * (b), (a.z) * (b)}

#define sb_vec3add3(a, b) (sb_vec3f) {(a.x) + (b.x), (a.y) + (b.y), (a.z) + (b.z)}
#define sb_vec3add1(a, b) (sb_vec3f) {(a.x) + (b), (a.y) + (b), (a.z) + (b)}

#endif //_SOFTMATH_HEADER
