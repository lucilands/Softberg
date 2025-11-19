#include <softberg/softberg.h>
#include <softberg/utils.h>

#include <SDL2/SDL.h>

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <time.h>


#define AVRG_FPS_NUM 100
#define ROTATION_SPEED 1

sb_transform transform = {
  .position = {1.0f, 1.0f, 1.0f},
  .rotation = {0.0f, 0.0f, 0.0f},
  .scale = {100.0f, 100.0f, 100.0f}
};

const sb_uint WIDTH = 1080;
const sb_uint HEIGHT = 720;



void render_depth(sb_canvas *canvas, SDL_Texture *tex) {
  float min_d = FLT_MAX, max_d = -FLT_MAX;
  for (sb_uint i = 0; i < canvas->width * canvas->height; i++) {
    if (canvas->depth[i] < min_d) min_d = canvas->depth[i];
    if (canvas->depth[i] > max_d) max_d = canvas->depth[i];
  }
  float range = (max_d - min_d > 0) ? (max_d - min_d) : 1.0f;

  void *pixels;
  int pitch;
  SDL_LockTexture(tex, NULL, &pixels, &pitch);
  uint8_t *dst = pixels;
  for (sb_uint y = 0; y < canvas->height; y++) {
    uint8_t* row = dst + y * pitch;
    for (sb_uint x = 0; x < canvas->width; x++) {
      sb_uint i = y * canvas->width + x;
      sb_uint j = x * 4;
      float v = (canvas->depth[i] - min_d) / range; // normalize to [0,1]
      uint8_t c = (uint8_t)(fminf(fmaxf(v, 0.0f), 1.0f) * 255);
      row[j+0] = c;
      row[j+1] = c;
      row[j+2] = c;
      row[j+3] = 255;
    }
  }
  SDL_UnlockTexture(tex);
}

int main() {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window* win = SDL_CreateWindow("Softberg Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
  SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
  SDL_Texture* tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);

  bool depth_screen = false, interpolate_colors = false;

  sb_canvas *canvas = sb_canvas_init(WIDTH, HEIGHT);
  if (!canvas) {printf("ERROR: Failed to create canvas: %s\n", strerror(errno)); return 1;}

  sb_mesh mesh = sb_load_obj("models/suzanne.obj"); 

  int running = 1;
  SDL_Event e;
  clock_t start, end;
  float delta_time = 0.0f, avrg_dt = 0.0f;
  sb_uint num_frames = 0;

  while (running) {
    start = clock();
    while (SDL_PollEvent(&e)) {
      switch(e.type) {
        case SDL_QUIT: running = 0; break;

        case SDL_KEYDOWN: {
          switch (e.key.keysym.sym) {
            case SDLK_d: depth_screen = !depth_screen; break;
            case SDLK_c: interpolate_colors = !interpolate_colors; break;
            case SDLK_s: sb_write_ppm(canvas, "snapshot.ppm"); break;
            case SDLK_q: running = false; break;
            default:break;
          }
          break;
        }
      }
    }
    sb_canvas_fill(canvas, (sb_color) {0, 0, 0, 255});
    sb_render_mesh(canvas, mesh, transform, interpolate_colors);

    //if (!depth_screen) render_canvas(canvas, tex);
    //else render_depth(canvas, tex);
    if (!depth_screen) SDL_UpdateTexture(tex, NULL, canvas->data, canvas->width * sizeof(sb_color));
    else render_depth(canvas, tex);

    transform.rotation.y += ROTATION_SPEED*delta_time;
    transform.rotation.x += ROTATION_SPEED*delta_time;

    SDL_RenderCopy(ren, tex, NULL, NULL);
    SDL_RenderPresent(ren);

    end = clock();
    delta_time = (double)(end - start) / CLOCKS_PER_SEC;
    avrg_dt += delta_time;
    num_frames++;

    if (num_frames % AVRG_FPS_NUM == 0) {
      printf("\r%.2ffps", 1.0f / (avrg_dt / AVRG_FPS_NUM));
      fflush(stdout);
      avrg_dt = 0.0f;
    }
  }

  SDL_DestroyTexture(tex);
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();
  sb_canvas_delete(canvas);
  sb_mesh_delete(mesh);
  return 0;
}
