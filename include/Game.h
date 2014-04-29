#ifndef INCLUDE_GAME_H
#define INCLUDE_GAME_H

#include "SDLWrapper.h"
#include "Window.h"
#include "Tile.h"
#include "Configuration.h"


const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;

/**
* Main structure class for the game.
* Contains all the necessary functionalities to loop and update the game.
* 	Is the state machine (controls current state), contains all the different possible states.
*/
class Game {

	public:
		/**
		* The constructor.
		* Sets the game window and tells the game that it is OK to begin looping. Also, it
		* 	begins the FPS manager.
		* @param window_ : a created Window.
		* @note If the Window parameter is null, the game will not begin.
		*/
		Game(Window* window_);

		/**
		* The destructor.
		* Destroys the game's Window and states, and unloads current state.
		*/
		~Game();
		
		/**
		* The main game loop.
		* Orders the game structure, such as inputs, updates, and rendering.
		*/
		void runGame();

		void clipTiles(SDL_Rect clips[TileCode::TOTAL]);
		bool setTiles(Tile** tile);
		void setCamera(SDL_Rect& camera);
		void saveTiles(Tile** tiles);
		void showType(int tileType_);
		void putTile(Tile** tiles, int tileType, SDL_Rect& camera);
		
	private:
		Window *window; /**< The game Window. */
		bool isRunning; /**< Whether the game is currently running/looping or not. */		
		FPSmanager fpsManager; /**< The FPSManager from SDL2_GFX. Handles the framerate
			capping. */

};

#endif //INCLUDE_GAME_H
