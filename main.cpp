#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_Image.h>
#include <vector>
#include <math.h>

#define TABLE_PRICOLOR (Uint8) 118, (Uint8) 150, (Uint8) 86, SDL_ALPHA_OPAQUE
#define TABLE_SECCOLOR (Uint8) 238, (Uint8) 238, (Uint8) 210, SDL_ALPHA_OPAQUE
#define WHITE_PAWNC (Uint8) 200, (Uint8) 200, (Uint8) 200, SDL_ALPHA_OPAQUE
#define BLACK_PAWNC (Uint8) 24, (Uint8) 24, (Uint8) 24, SDL_ALPHA_OPAQUE
#define TABLE_SELECTEDCOLOR (Uint8) 183, (Uint8) 53, (Uint8) 35, SDL_ALPHA_OPAQUE

using namespace std;

typedef enum Type {
  Pawn,
  Knight,
  Bishop,
  Rook,
  Queen,
  King
} Type;

typedef enum PlayerColors {
  PBLACK, PWHITE
} PlayerColors;

typedef struct PieceLoc {
  int i;
  PlayerColors color;
} PieceLoc;

class Piece {
  int x, y;
  Type type;
  public:
    Piece(int xarg, int yarg, Type typearg) {
      x = xarg;
      y = yarg;
      type = typearg;
    }
    int GetX() {
      return x;
    }
    int GetY() {
      return y;
    }
    Type GetType() {
      return type;
    }
};

class Game {
  vector<Piece> WhitePieces, BlackPieces;
  int MoveCount;
  PlayerColors turn;
  public:
    PieceLoc Selected;
    vector<Piece> StartingPosition(PlayerColors color) {
      vector<Piece> res;
      for(int i = 0; i < 8; i++) {
        Piece pawn(i, color == PWHITE ? 1 : 6, Pawn);
        res.push_back(pawn);
      }
      int ypos = color == PWHITE ? 0 : 7;
      Piece Rook1(0, ypos, Rook);
      Piece Knight1(1, ypos, Knight);
      Piece Bishop1(2, ypos, Bishop);
      Piece Queen1(3, ypos, Queen);
      Piece King1(4, ypos, King);
      Piece Bishop2(5, ypos, Bishop);
      Piece Knight2(6, ypos , Knight);
      Piece Rook2(7, ypos, Rook);
      res.push_back(Rook1);
      res.push_back(Knight1);
      res.push_back(Bishop1);
      res.push_back(Queen1);
      res.push_back(King1);
      res.push_back(Bishop2);
      res.push_back(Knight2);
      res.push_back(Rook2);
      return res;
    }
    Game() {
      turn = PWHITE;
      MoveCount = 0;
      WhitePieces = StartingPosition(PWHITE);
      BlackPieces = StartingPosition(PBLACK);
      Selected = (PieceLoc) {-1, PWHITE};
    }
    vector<Piece> GetPieces(PlayerColors color) {
      if(color == PWHITE) {
        return WhitePieces;
      }else {
        return BlackPieces;
      }
    }
    Piece GetPieceInSquare(int x, int y) {
      for(size_t i = 0; i < WhitePieces.size(); i++) {
        if(WhitePieces[i].GetX() == x && WhitePieces[i].GetY() == y) {
          return WhitePieces[i];
        }
      }
      for(size_t i = 0; i < BlackPieces.size(); i++) {
        if(BlackPieces[i].GetX() == x && BlackPieces[i].GetY() == y) {
          return BlackPieces[i];
        }
      }
      Piece empty(-1,-1,King);
      return empty;
    }
    PieceLoc GetPieceLocInSquare(int x, int y) {
      PieceLoc res;
      for(size_t i = 0; i < WhitePieces.size(); i++) {
        if(WhitePieces[i].GetX() == x && WhitePieces[i].GetY() == y) {
          res.color = PWHITE;
          res.i = i;
          return res;
        }
      }
      for(size_t i = 0; i < BlackPieces.size(); i++) {
        if(BlackPieces[i].GetX() == x && BlackPieces[i].GetY() == y) {
          res.color = PBLACK;
          res.i = i;
          return res;
        }
      }
      res.i = -1;
      return res;
    }
};

const int WINDOW_WIDTH = 1000, WINDOW_HEIGHT = 800, SQUARE_SIZE = 100, PIECE_SIZE = 90;

Piece FindPiece(int x, int y, Game game) {
  // FIXME: Satranç tahtası yer değiştirince bozulacak.
  return game.GetPieceInSquare((int) floor(x/SQUARE_SIZE), (int) floor(y/SQUARE_SIZE));
}


PieceLoc FindPieceLoc(int x, int y, Game game) {
  // FIXME: Satranç tahtası yer değiştirince bozulacak.
  return game.GetPieceLocInSquare((int) floor(x/SQUARE_SIZE), (int) floor(y/SQUARE_SIZE));
}

void RenderTable(SDL_Renderer* renderer) {
  for(int i = 0; i < 8; i++) {
    for(int j = 0; j < 8; j++) {
      if(((1-(i%2))+j)%2 == 0) {
        SDL_SetRenderDrawColor(renderer, TABLE_PRICOLOR);
      }else {
        SDL_SetRenderDrawColor(renderer, TABLE_SECCOLOR);
      }
      using sdlrect = SDL_Rect;
      const SDL_Rect rect = sdlrect {j*SQUARE_SIZE, i*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE};
      SDL_RenderFillRect(renderer, &rect);
    }
  }
}

void RenderPiece(SDL_Renderer* renderer, Piece p, PlayerColors color) {
  SDL_Texture* newTexture = NULL;
  // Mostly stolen from https://lazyfoo.net/tutorials/SDL/07_texture_loading_and_rendering/index.php
  //Load image at specified path
  string pip;
  switch(p.GetType()) {
    case Pawn: {
      if(color == PWHITE) {
        pip = "./pieces/white_pawn.png";
      }else {
        pip = "./pieces/black_pawn.png";
      }
      break;
    }
    case Rook: {
      if(color == PWHITE) {
        pip = "./pieces/white_rook.png";
      }else {
        pip = "./pieces/black_rook.png";
      }
      break;
    }
    case Knight: {
      if(color == PWHITE) {
        pip = "./pieces/white_knight.png";
      }else {
        pip = "./pieces/black_knight.png";
      }
      break;
    }
    case Bishop: {
      if(color == PWHITE) {
        pip = "./pieces/white_bishop.png";
      }else {
        pip = "./pieces/black_bishop.png";
      }
      break;
    }
    case Queen: {
      if(color == PWHITE) {
        pip = "./pieces/white_queen.png";
      }else {
        pip = "./pieces/black_queen.png";
      }
      break;
    }
    case King: {
      if(color == PWHITE) {
        pip = "./pieces/white_king.png";
      }else {
        pip = "./pieces/black_king.png";
      }
      break;
    }
    default: {
      pip = "./pieces/black_king.png";
      break;
    }
  }
  SDL_Surface* loadedSurface = IMG_Load(pip.c_str());
  if( loadedSurface == NULL )
  {
      printf( "Unable to load image %s! SDL_image Error: %s\n", pip.c_str(), IMG_GetError() );
  }
  else
  {
      newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
      if( newTexture == NULL )
      {
          printf( "Unable to create texture from %s! SDL Error: %s\n", pip.c_str(), SDL_GetError() );
      }
      using sdlrect = SDL_Rect;
      const SDL_Rect rect = sdlrect {p.GetX()*SQUARE_SIZE+5, p.GetY()*SQUARE_SIZE+5, PIECE_SIZE, PIECE_SIZE};
      SDL_RenderCopy( renderer, newTexture, NULL, &rect);
      SDL_DestroyTexture(newTexture);
      newTexture = NULL;
      SDL_FreeSurface( loadedSurface );
  }
}

void RenderPieces(SDL_Renderer* renderer, Game game) {
  if(game.Selected.i != -1) {
      int x = game.GetPieces(game.Selected.color)[game.Selected.i].GetX();
      int y = game.GetPieces(game.Selected.color)[game.Selected.i].GetY();
      SDL_SetRenderDrawColor(renderer, TABLE_SELECTEDCOLOR);
      using sdlrect = SDL_Rect;
      const SDL_Rect rect = sdlrect {x*SQUARE_SIZE, y*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE};
      SDL_RenderFillRect(renderer, &rect);
  }
  vector<Piece> whitep = game.GetPieces(PWHITE);
  for(size_t i = 0; i < whitep.size(); i++) {
    /* SDL_SetRenderDrawColor(renderer, WHITE_PAWNC); */
    /* using sdlrect = SDL_Rect; */
    /* const SDL_Rect rect = sdlrect {whitep[i].GetX()*SQUARE_SIZE+5, whitep[i].GetY()*SQUARE_SIZE+5, PIECE_SIZE, PIECE_SIZE}; */
    /* SDL_RenderFillRect(renderer, &rect); */
    RenderPiece(renderer, whitep[i], PWHITE);
  }
  vector<Piece> blackp = game.GetPieces(PBLACK);
  for(size_t i = 0; i < whitep.size(); i++) {
    /* SDL_SetRenderDrawColor(renderer, BLACK_PAWNC); */
    /* using sdlrect = SDL_Rect; */
    /* const SDL_Rect rect = sdlrect {blackp[i].GetX()*SQUARE_SIZE+5, blackp[i].GetY()*SQUARE_SIZE+5, PIECE_SIZE, PIECE_SIZE}; */
    /* SDL_RenderFillRect(renderer, &rect); */
    RenderPiece(renderer, blackp[i], PBLACK);
  }
}

int main(int argc, char** argv) {
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window *window = SDL_CreateWindow("Chessdl", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
  IMG_Init(IMG_INIT_PNG);
  Game *game = new Game();

  int quit = 0;
  SDL_Event event;
  while(quit == 0) {
    SDL_PollEvent(&event);
    switch(event.type) {
      case SDL_QUIT: {
        quit = 1;
        break;
      }
      case SDL_MOUSEBUTTONDOWN: {
        game->Selected = FindPieceLoc(event.button.x, event.button.y, *game);
        break;
      }
    }
    SDL_SetRenderDrawColor(renderer, (Uint8) 0, (Uint8)0, (Uint8)0, (Uint8)255);
    SDL_RenderClear(renderer);
    RenderTable(renderer);
    RenderPieces(renderer, *game);
    SDL_RenderPresent(renderer);
  }
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
