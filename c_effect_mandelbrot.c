#include "c_effect.h"

static
color_t mandelbrot_px(uint64_t x, uint64_t y, uint64_t width, uint64_t height, uint32_t frames, void* ctx) {
	float cx = x / (float) width * 2.0 - 1.0;
	float cy = y / (float) height * 2.0 - 1.0;

	float px = 0;
	float py = 0;
	for (int i = 0; i < 16; i++) {
		float new_px = px * px - py * py + cx;
		float new_py = px * py + py * px + cy;
		px = new_px;
		py = new_py;
	}
	float fi = (px * px + py * py) * 255;
	int i = ((int)fi) % 255;

	return (color_t) { i, i, i, 255 };
}

struct c_effect c_effect_mandelbrot = {
	.vtable = {
		.render = c_effect__render,
		.input = NULL
	},
	.draw_pixel = mandelbrot_px,
	.ctx = NULL
};

