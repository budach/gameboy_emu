COMPILER = g++
COMMONFLAGS = -Wall -Wextra -Werror -Wshadow -Wdouble-promotion -Wpedantic -Wformat=2 -pipe -std=c++20
DEBUGFLAGS = -O0 -g3
RELEASEFLAGS = -flto -march=native -O3 -s
LDFLAGS = -lraylib -lopengl32 -lgdi32 -lwinmm

FILES = src/main.cpp src/gameboy.cpp src/mmu.cpp src/opcodes.cpp src/ppu.cpp
EXECUTABLE = gameboy.exe

release:
	$(COMPILER) $(COMMONFLAGS) $(RELEASEFLAGS) $(FILES) -o $(EXECUTABLE) $(LDFLAGS)
	strip --strip-all -R .comment -R .note $(EXECUTABLE)

debug:
	$(COMPILER) $(COMMONFLAGS) $(DEBUGFLAGS) $(FILES) -o $(EXECUTABLE) $(LDFLAGS)