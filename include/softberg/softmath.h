#ifndef _SOFTMATH_HEADER
#define _SOFTMATH_HEADER

typedef unsigned int sb_uint;
typedef unsigned char sb_uint8;

typedef struct {
  sb_uint8 r, g, b, a;
} sb_color;

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
  sb_vec3f position;
  sb_color color;
  sb_vec3f normal;
  sb_vec2f texture_coordinate;
} sb_vertex;

typedef struct {
  sb_vec3f v1, v2, v3;
  sb_color v1_color, v2_color, v3_color;
} sb_triangle3d;

typedef struct {
  sb_vec2f v1, v2, v3;
  sb_color c1, c2, c3;
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

#define lerp(a, b, x) _Generic((a), \
			float: lerp2f, \
			sb_vec2f: lerp2f2, \
			sb_vec3f: lerp2f3, \
			sb_color: lerp2c, \
			sb_uint: lerp2u, \
			sb_uint8: lerp2u8 \
		)(a, b, x)

#define lerp3(a, b, c, x) _Generic((a), \
			float: lerp3f, \
			sb_vec2f: lerp3f2, \
			sb_vec3f: lerp3f3, \
			sb_color: lerp3c, \
			sb_uint: lerp3u, \
			sb_uint8: lerp3u8 \
		)(a, b, c, x)


float lerp2f(float a, float b, float x);
sb_vec2f lerp2f2(sb_vec2f a, sb_vec2f b, float x);
sb_vec3f lerp2f3(sb_vec3f a, sb_vec3f b, float x);
sb_color lerp2c(sb_color a, sb_color b, float x);
sb_uint lerp2u(sb_uint a, sb_uint b, sb_uint x);
sb_uint8 lerp2u8(sb_uint8 a, sb_uint8 b, sb_uint8 x);

float lerp3f(float a, float b, float c, float x);
sb_vec2f lerp3f2(sb_vec2f a, sb_vec2f b, sb_vec2f c, float x);
sb_vec3f lerp3f3(sb_vec3f a, sb_vec3f b, sb_vec3f c, float x);
sb_color lerp3c(sb_color a, sb_color b, sb_color c, float x);
sb_uint lerp3u(sb_uint a, sb_uint b, sb_uint c, float x);
sb_uint8 lerp3u8(sb_uint8 a, sb_uint8 b, sb_uint8 c, float x);

sb_color barylerp(sb_triangle2d tri, sb_vec2f p);

#endif //_SOFTMATH_HEADER
