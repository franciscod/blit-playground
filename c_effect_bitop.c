#include "c_effect.h"

static
color_t bitop_px(uint64_t x, uint64_t y, uint64_t width, uint64_t height, uint32_t frames, void* ctx) {
	return (color_t) { x^y, x&y, x|y, 255 };
}

struct c_effect c_effect_bitop = {
	.vtable = {
		.render = c_effect__render,
		.input = NULL
	},
	.draw_pixel = bitop_px,
	.ctx = NULL
};

