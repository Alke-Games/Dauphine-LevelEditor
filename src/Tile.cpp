#include "Tile.h"
#include "Logger.h"

Tile::Tile(int x_, int y_, int tileType_) :
	type(tileType_)
{
	this->rectangle = {x_, y_, TILE_SIZE, TILE_SIZE};
}

Tile::~Tile(){

}

SDL_Rect& Tile::getRectangle(){
	return this->rectangle;
}

int Tile::getType(){
	return this->type;
}
