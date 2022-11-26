CC=g++
CFLAGS=-Wall -Wextra -Wshadow -pedantic -ggdb -Wno-unused-parameter `pkg-config --cflags sdl2`
LIBS=`pkg-config --libs sdl2` -lm

main: main.cpp
	$(CC) $(CFLAGS) -o main.exe main.cpp $(LIBS)
