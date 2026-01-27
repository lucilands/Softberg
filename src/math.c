#include <softberg/softmath.h>


inline float lerp2f(float a, float b, float x) {
	return a + x * (b - a);
}

inline sb_uint lerp2u(sb_uint a, sb_uint b, sb_uint x) {
	return a + x * (b - a);
}

inline sb_uint8 lerp2u8(sb_uint8 a, sb_uint8 b, sb_uint8 x) {
	return a + x * (b - a);
}

inline sb_vec2f lerp2f2(sb_vec2f a, sb_vec2f b, float x) {
	return sb_vec2(lerp2f(a.x, b.x, x), lerp2f(a.y, b.y, x));
}

inline sb_vec3f lerp2f3(sb_vec3f a, sb_vec3f b, float x) {
	return sb_vec3(lerp2f(a.x, b.x, x), lerp2f(a.y, b.y, x), lerp2f(a.z, b.z, x));
}

inline sb_color lerp2c(sb_color a, sb_color b, float x) {
	return (sb_color) {lerp2u(a.r, b.r, x), lerp2u(a.g, b.g, x), lerp2u(a.b, b.b, x), lerp2u(a.a, b.a, x)};
}

inline float lerp3f(float a, float b, float c, float x) {
	if (x <= 0.5f) return lerp2f(a, b, x);
	return lerp2f(b, c, x);
}

inline sb_vec2f lerp3f2(sb_vec2f a, sb_vec2f b, sb_vec2f c, float x) {
	return sb_vec2(lerp3f(a.x, b.x, c.x, x), lerp3f(a.y, b.y, c.y, x));
}

inline sb_vec3f lerp3f3(sb_vec3f a, sb_vec3f b, sb_vec3f c, float x) {
	return sb_vec3(lerp3f(a.x, b.x, c.x, x), lerp3f(a.y, b.y, c.y, x), lerp3f(a.z, b.z, c.z, x));
}

inline sb_uint lerp3u(sb_uint a, sb_uint b, sb_uint c, float x) {
	if (x <= 0.5f) return lerp2u(a, b, x);
	return lerp2u(b, c, x);
}

inline sb_uint8 lerp3u8(sb_uint8 a, sb_uint8 b, sb_uint8 c, float x) {
	if (x <= 0.5f) return lerp2u8(a, b, x);
	return lerp2u8(b, c, x);
}

inline sb_color lerp3c(sb_color a, sb_color b, sb_color c, float x) {
	return (sb_color) {lerp3u(a.r, b.r, c.r, x), lerp3u(a.g, b.g, c.g, x), lerp3u(a.b, b.b, c.b, x), lerp3u(a.a, b.a, c.a, x)};
}

inline float edge(sb_vec2f a, sb_vec2f b, sb_vec2f p) {
    return (p.x - a.x) * (b.y - a.y) -
           (p.y - a.y) * (b.x - a.x);
}

inline void barycentric(sb_vec2f v0, sb_vec2f v1, sb_vec2f v2, sb_vec2f p, float *w0, float *w1, float *w2) {
    float area = edge(v0, v1, v2);

    float a0 = edge(v1, v2, p);
    float a1 = edge(v2, v0, p);
    float a2 = edge(v0, v1, p);

    float inv_area = 1.0f / area;

    *w0 = a0 * inv_area;
    *w1 = a1 * inv_area;
    *w2 = a2 * inv_area;
}

inline sb_color barylerp(sb_triangle2d tri, sb_vec2f p) {
	float w0, w1, w2;

	barycentric(tri.v1, tri.v2, tri.v3, p, &w0, &w1, &w2);
	return (sb_color){
        tri.c1.r * w0 + tri.c2.r * w1 + tri.c3.r * w2,
        tri.c1.g * w0 + tri.c2.g * w1 + tri.c3.g * w2,
        tri.c1.b * w0 + tri.c2.b * w1 + tri.c3.b * w2,
		tri.c1.a * w0 + tri.c2.a * w1 + tri.c3.a * w2
    };
}

