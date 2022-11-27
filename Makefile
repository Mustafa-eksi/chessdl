CC=g++
CFLAGS=-Wall -Wextra -Wshadow -pedantic -ggdb -Wno-unused-parameter `pkg-config --cflags sdl2 SDL2_image`
LIBS=`pkg-config --libs sdl2 SDL2_image` -lm

main: main.cpp
	$(CC) $(CFLAGS) -o main.exe main.cpp $(LIBS)
