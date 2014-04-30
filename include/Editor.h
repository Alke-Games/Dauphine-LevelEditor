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

		static string mapName;
		static int numberOfTiles;

	private:
		void clipTiles();
		bool setTiles();
		void setCamera();
		void saveTiles();
		void showType();
		void putTile();
		void countTiles();

		SDL_Rect clips[TileCode::TOTAL];
		Sprite* tileSheet;
		Tile** tiles;
		int currentType;
		SDL_Rect camera;

};

#endif // INCLUDE_EDITOR_H
