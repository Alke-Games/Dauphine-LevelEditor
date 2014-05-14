#ifndef INCLUDE_TILE_H
#define INCLUDE_TILE_H

#include "SDLWrapper.h"

//Tile constants
const int TILE_SIZE = 64;
const int TILE_PER_ROW_IMAGE = 5;
//const int TILE_SPRITES = 10; --> TileCode::Total

//The different tile sprites
enum TileCode : int {
	FLOOR1 = 0,
	FLOOR2,
	WALL1,
	WALL2,
	WALL3,
	WALL4,
	WALL5,
	WALL6,
	WALL7,
	WALL8,
	WALL9,
	WALL10,
	WALL11,
	WALL12,
	WALL13,
	TOTAL
};

class Tile {

	public:

		Tile(int x_, int y_, int tileType_);
		~Tile();
		SDL_Rect& getRectangle();
		int getType();

	private:
		int type;
		SDL_Rect rectangle;


};

#endif // INCLUDE_TILE_H
