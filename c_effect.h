#pragma once
#include "common.h"

struct c_effect {
	struct effect_vtable vtable;
	color_t (*draw_pixel)(uint64_t x, uint64_t y, uint64_t width, uint64_t height, uint32_t frames, void* ctx);
	void* ctx;
};

void c_effect__render(struct effect* self, color_t* canvas, uint64_t width, uint64_t height, uint32_t frames);
