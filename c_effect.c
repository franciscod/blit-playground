#include "c_effect.h"

void c_effect__render(struct effect* self, color_t* canvas, uint64_t width, uint64_t height, uint32_t frames) {
	struct c_effect* myself = (struct c_effect*) self;
	uint8_t bytes_per_pixel = 4;

	for (uint64_t y = 0; y < height; y++) {
		for (uint64_t x = 0; x < width; x++) {
			canvas[x + y * width] = myself->draw_pixel(x, y, width, height, frames, myself->ctx);
		}
	}
}

