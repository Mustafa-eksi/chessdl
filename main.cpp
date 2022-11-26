#include <iostream>
#include <SDL2/SDL.h>
#include <vector>

#define TABLE_PRICOLOR (Uint8) 118, (Uint8) 150, (Uint8) 86, SDL_ALPHA_OPAQUE
#define TABLE_SECCOLOR (Uint8) 238, (Uint8) 238, (Uint8) 210, SDL_ALPHA_OPAQUE

using namespace std;

class Piece {
  typedef enum Type {
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King
  };
  int x, y;
  Type type;
};

class Game {
  typedef enum PlayerColors {
    PBLACK, PWHITE
  };
  vector<Piece> pieces;
  int MoveCount;
  PlayerColors turn;
};

const int WINDOW_WIDTH = 1000, WINDOW_HEIGHT = 800, SQUARE_SIZE = 100;

void RenderTable(SDL_Renderer* renderer) {
  for(int i = 0; i < 8; i++) {
    for(int j = 0; j < 8; j++) {
      if(((1-(i%2))+j)%2 == 0) {
        SDL_SetRenderDrawColor(renderer, TABLE_PRICOLOR);
      }else {
        SDL_SetRenderDrawColor(renderer, TABLE_SECCOLOR);
      }
      const SDL_Rect rect = (SDL_Rect) {j*SQUARE_SIZE, i*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE};
      SDL_RenderFillRect(renderer, &rect);
    }
  }
}

int main(int argc, char** argv) {
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window *window = SDL_CreateWindow("Chessdl", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
  int quit = 0;
  SDL_Event event;
  while(quit == 0) {
    SDL_PollEvent(&event);
    switch(event.type) {
      case SDL_QUIT: {
        quit = 1;
        break;
      }
    }
    SDL_SetRenderDrawColor(renderer, (Uint8) 0, (Uint8)0, (Uint8)0, (Uint8)255);
    SDL_RenderClear(renderer);
    RenderTable(renderer);
    SDL_RenderPresent(renderer);
  }
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
