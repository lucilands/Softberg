# Softberg - 3D software rasterizer

Softberg is a 3D rasterizer that runs on the CPU.
It will render into a buffer stored in RAM, and can be used for everything from rendering 3D models in the console to a fully fledged quake style game.

## Usage

Softberg uses a buffer called the *canvas*, this is where all read/write calls go to, and what Softberg uses for rendering. Similar to OpenGL [framebuffers](https://learnopengl.com/Advanced-OpenGL/Framebuffers).

The goal of Sofberg is having as little boilerplate as possible.
Creating and deleting a canvas looks like this, and all rendering calls happen in between these two lines.

```C
#include <softberg/softberg.h>


int main() {
    sb_canvas *canvas = sb_canvas_init(1080, 720);
    
    sb_canvas_delete(canvas);
    return 0;
}
```

Softberg defines a couple utilities in `softberg/utils.h`, for this example we're going to take a look at the `sb_write_ppm` utility function.

```C
#include <softberg/softberg.h>
#include <softberg/utils.h>


int main() {
    sb_canvas *canvas = sb_canvas_init(1080, 720);

    // This will write the canvas to a ppm file called "output.ppm".
    // Since nothing has been drawn to the canvas this will be a black image.
    sb_write_ppm(canvas, "output.ppm");
    sb_canvas_delete(canvas);
    return 0;
}
```


