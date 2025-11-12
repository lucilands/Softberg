#include <softberg/softberg.h>
#include <softberg/utils.h>

#include <SDL2/SDL.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>



sb_transform transform = {
  .position = {1.0f, 1.0f, 1.0f},
  .rotation = {0.0f, 0.0f, 0.0f},
  .scale = {100.0f, 100.0f, 100.0f}
};

const sb_uint WIDTH = 1080;
const sb_uint HEIGHT = 720;

int main() {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window* win = SDL_CreateWindow("Softberg Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
  SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
  SDL_Texture* tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);

  sb_canvas *canvas = sb_canvas_init(WIDTH, HEIGHT);
  if (!canvas) {printf("ERROR: Failed to create canvas: %s\n", strerror(errno)); return 1;}

  sb_mesh mesh = sb_load_obj("models/cube.obj");  

  int running = 1;
  SDL_Event e;
  while (running) {
    while (SDL_PollEvent(&e)) if (e.type == SDL_QUIT) running = 0;
    sb_canvas_fill(canvas, (sb_color) {0, 0, 0});
    sb_render_mesh(canvas, mesh, transform);

    void* pixels;
    int pitch;
    SDL_LockTexture(tex, NULL, &pixels, &pitch);
    uint8_t* dst = pixels;

    for (sb_uint y = 0; y < canvas->height; y++) {
      uint8_t* row = dst + y * pitch;
      for (sb_uint x = 0; x < canvas->width; x++) {
        sb_color c = canvas->data[y * canvas->width + x];
        // Clamp to 0-255
        uint8_t r = (c.r > 255 ? 255 : c.r);
        uint8_t g = (c.g > 255 ? 255 : c.g);
        uint8_t b = (c.b > 255 ? 255 : c.b);

        // RGBA8888 expects R,G,B,A in this order
        row[x*4 + 0] = r;
        row[x*4 + 1] = g;
        row[x*4 + 2] = b;
        row[x*4 + 3] = 255; // full alpha
      }
    }

    SDL_UnlockTexture(tex);

    transform.rotation.y += 0.025;
    transform.rotation.x += 0.025;

    SDL_RenderClear(ren);
    SDL_RenderCopy(ren, tex, NULL, NULL);
    SDL_RenderPresent(ren);
  }

  SDL_DestroyTexture(tex);
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();
  sb_canvas_delete(canvas);
  return 0;
}
