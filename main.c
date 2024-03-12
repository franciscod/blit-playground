#include <SDL.h>
#undef main

#include "common.h"

static
void update_keyboard(struct keyboard_state* key, SDL_KeyboardEvent ev) {
	bool state = ev.state == SDL_PRESSED;

	switch (ev.keysym.scancode) {
	case SDL_SCANCODE_W:
		key->W = state;
		break;
	case SDL_SCANCODE_A:
		key->A = state;
		break;
	case SDL_SCANCODE_S:
		key->S = state;
		break;
	case SDL_SCANCODE_D:
		key->D = state;
		break;
	case SDL_SCANCODE_SPACE:
		key->Space = state;
		break;
	case SDL_SCANCODE_LCTRL:
		key->LCtrl = state;
		break;
	case SDL_SCANCODE_UP:
		key->Up = state;
		break;
	case SDL_SCANCODE_DOWN:
		key->Down = state;
		break;
	case SDL_SCANCODE_LEFT:
		key->Left = state;
		break;
	case SDL_SCANCODE_RIGHT:
		key->Right = state;
		break;
	}
}

int enforce_canvas_size(SDL_Surface** canvas, SDL_Surface* win) {
	if (*canvas == NULL || (*canvas)->w != win->w || (*canvas)->h != win->h) {
		LOG("Requesting new canvas (%dx%d)", win->w, win->h);
		SDL_Surface* new_canvas = SDL_CreateRGBSurfaceWithFormat(0, win->w, win->h, 32, SDL_PIXELFORMAT_RGBA32);
		if (new_canvas == NULL) return 1;
		SDL_FreeSurface(*canvas);
		*canvas = new_canvas;
	}
	return 0;
}

// Known c_effects
extern struct c_effect c_effect_bitop;
extern struct c_effect c_effect_mandelbrot;

struct c_effect *c_effects_all[] = {
	&c_effect_mandelbrot,
	&c_effect_bitop,
};

#define C_EFFECTS_COUNT (sizeof(c_effects_all) / sizeof(c_effects_all[0]))

size_t c_effects_current_index = 0;

int main(int argc, const char* argv[]) {
	struct keyboard_state key = {0};

	SDL_Window* win;
	SDL_Surface* surf;
	SDL_Surface* canvas = NULL;
	SDL_Event event;

	/* Performance counters */
	uint32_t frames = 0;
	uint32_t frame_min = 0xFFFFFFFF;
	uint32_t frame_max = 0x0;
	uint32_t frame_total = 0;

	effect_t* current_effect = (effect_t*) c_effects_all[c_effects_current_index];

	LOG("Inicializando SDL");
	if (SDL_Init(SDL_INIT_VIDEO))
		die(SDL_GetError());

	win = SDL_CreateWindow("Dibu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 512, SDL_WINDOW_RESIZABLE);
	if (win == NULL)
		die(SDL_GetError());

	while (1) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				goto exit;
			if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_TAB) {
				current_effect = (effect_t *) c_effects_all[(++c_effects_current_index) % C_EFFECTS_COUNT];
				continue;
			}
			if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
				update_keyboard(&key, event.key);
		}
		if (current_effect->vtable.input != NULL)
			current_effect->vtable.input(current_effect, &key, frames);

		surf = SDL_GetWindowSurface(win);
		if (surf == NULL)
			die(SDL_GetError());

		if (enforce_canvas_size(&canvas, surf))
			die(SDL_GetError());

		if (SDL_MUSTLOCK(canvas))
			SDL_LockSurface(canvas);

		uint32_t frame_start = SDL_GetTicks();
		if (current_effect->vtable.render != NULL)
			current_effect->vtable.render(current_effect, canvas->pixels, canvas->w, canvas->h, frames);
		uint32_t frame_end = SDL_GetTicks();

		frames++;
		frame_total += frame_end - frame_start;
		frame_min = MIN(frame_min, frame_end - frame_start);
		frame_max = MAX(frame_max, frame_end - frame_start);

		if (frames % 100 == 0) {
			LOG("Frame %d\ttime %d", frames, frame_end - frame_start);
			LOG("min %d\tmax %d\tavg %f", frame_min, frame_max,
			    frame_total / (float) frames);
		}

		if (SDL_MUSTLOCK(canvas))
			SDL_UnlockSurface(canvas);

		SDL_BlitSurface(canvas, NULL, surf, NULL);
		SDL_UpdateWindowSurface(win);
	}
exit:
	LOG("Cerrando");
	if (canvas != NULL)
		SDL_FreeSurface(canvas);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
