CFLAGS += $$(sdl2-config --cflags) -std=c11
LDFLAGS += $$(sdl2-config --libs)

C_EFFECTS = c_effect.o c_effect_bitop.o c_effect_mandelbrot.o

exe: main.o $(C_EFFECTS)
	$(CC) $^ $(LDFLAGS) -o $@

run: exe
	./exe

clean:
	rm -f exe *.o

.PHONY: clean run
