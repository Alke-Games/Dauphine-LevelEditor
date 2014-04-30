#ifndef INCLUDE_EDITOR_H
#define INCLUDE_EDITOR_H

#include "SDLWrapper.h"
#include "Sprite.h"
#include "Tile.h"

class Editor {

	public:
		Editor();
		~Editor();
		void update();
		void render();

	private:
		void clipTiles();
		bool setTiles();
		void setCamera();
		void saveTiles();
		void showType();
		void putTile();

		SDL_Rect clips[TileCode::TOTAL];
		Sprite* tileSheet;
		Tile* tiles[TOTAL_TILES];
		int currentType;
		SDL_Rect camera;

};

#endif // INCLUDE_EDITOR_H