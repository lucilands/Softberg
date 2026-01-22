#include <softberg/softberg.h>
#include <softberg/utils.h>

#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


sb_transform transform = {
  .position = {0.0f, 0.0f, 0.0f},
  .rotation = {0.0f, 0.0f, 0.0f},
  .scale = {30.0f, 30.0f, 30.0f}
};

static _Bool running = 1;


void print_canvas(sb_canvas *canvas) {
	for (sb_uint i = (canvas->width * canvas->height) - 1; i > 0; i--) {
		sb_color pixel = canvas->data[i];
		printf("\x1b[48;2;%u;%u;%um ", pixel.r, pixel.g, pixel.b);
		//if (i % canvas->width == 0 && i > 0) printf("\n");
		fflush(stdout);
	}
}

void sigint_handler(int _) {
	running = 0;
}


int main(int argc, char *argv[]) {
	struct winsize termsize;
	ioctl(STDIN_FILENO, TIOCGWINSZ, &termsize);
	sb_canvas *canvas = sb_canvas_init(termsize.ws_col, termsize.ws_row);

	printf("terminal dimensions: %ix%i\n", termsize.ws_col, termsize.ws_row);

	signal(SIGINT, sigint_handler);

	if (argc < 2) {
		printf("ERROR: %s needs one argument, [model]\n", argv[0]);
		return 1;
	}
	sb_mesh mesh = sb_load_obj(argv[1]);

	system("tput smcup");

	char c = 0;

	system("tput civis");
	system ("/bin/stty raw");
	while (running && (c = getchar()) != 'q') {
		system("tput cup 0 0");
		sb_canvas_fill(canvas, (sb_color) {0, 0, 0, 255});
		sb_render_mesh(canvas, mesh, transform, 1);

		switch (c) {
			case 'a':
				transform.rotation.x -= 0.1;
				break;
			case 'd':
				transform.rotation.x += 0.1;
				break;
			case 'w':
				transform.rotation.y += 0.1;
				break;
			case 's':
				transform.rotation.y -= 0.1;
				break;
		}

		print_canvas(canvas);
	}
	sb_canvas_delete(canvas);
	system ("/bin/stty cooked");
	system("tput cnorm");
	system("tput rmcup");
}
