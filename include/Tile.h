#ifndef INCLUDE_TILE_H
#define INCLUDE_TILE_H

#include "SDLWrapper.h"

const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;

//Tile constants
const int TILE_WIDTH = 48;
const int TILE_HEIGHT = 48;
const int TILE_PER_ROW_IMAGE = 5;
//const int TILE_SPRITES = 10; --> TileCode::Total

//The different tile sprites
enum TileCode : int {
	BLACK = 0,
	WHITE,
	RED,
	GREEN,
	BLUE,
	YELLOW,
	LIGHTBLUE,
	PINK,
	DARKGRAY,
	GRAY,
	TOTAL
};

class Tile {

	public:

		Tile(int x_, int y_, int tileType_);
		~Tile();
		// void render(SDL_Rect& clipCamera, SDL_Rect clips[TileCode::TOTAL]);
		SDL_Rect& getRectangle();
		int getType();

	private:
		// bool checkCollision( SDL_Rect &A, SDL_Rect &B );
		int type;
		SDL_Rect rectangle;


};

#endif // INCLUDE_TILE_H
