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

		static int LEVEL_WIDTH;
		static int LEVEL_HEIGHT;

	private:
		void clipTiles();
		bool setTiles();
		void setCamera();
		void saveTiles();
		void putTile();
		void countTiles();
		bool checkCollision(SDL_Rect &A, SDL_Rect &B);
		void renderCurrentTileOnCursor();

		SDL_Rect clips[TileCode::TOTAL];
		Sprite* tileSheet;
		Tile** tiles;
		int currentType;
		SDL_Rect camera;

};

#endif // INCLUDE_EDITOR_H
