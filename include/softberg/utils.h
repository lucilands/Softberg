#ifndef __SOFTBERG_UTILS_HEADER
#include "softberg.h"

int sb_write_ppm(sb_canvas *canvas, const char * restrict filepath);
int sb_write_depth_ppm(sb_canvas *canvas, const char * restrict filepath);
sb_mesh sb_load_obj(const char * restrict path);

#endif // __SOFTBERG_UTILS_HEADER
