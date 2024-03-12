#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define LOG(format, ...) printf("[" __FILE__ ":%d] " format "\n", __LINE__, ##__VA_ARGS__)
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef struct {
	uint8_t r, g, b, a;
} color_t;

static
void die(const char* str) {
	perror(str);
	exit(-1);
}

/* key -> up/down */
struct keyboard_state {
	bool W, A, S, D;		/* Origin */
	bool Left, Right, Up, Down;	/*  Zoom  */
	bool Space, LCtrl;		/* Clean  */
};

struct effect;

struct effect_vtable {
	void (*render)(struct effect* self, color_t* canvas, uint64_t width, uint64_t height, uint32_t frames);
	void (*input)(struct effect* self, const struct keyboard_state* keyboard, uint32_t frames);
};

typedef struct effect {
	struct effect_vtable vtable;
} effect_t;
