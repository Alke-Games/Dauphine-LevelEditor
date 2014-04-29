#include "SDLWrapper.h"
#include "Logger.h"

bool SDLWrapper::initialize(){
	bool initSDL = false;
	bool initIMG = false;

	// Initializing SDL with initFlags.
	const Uint32 initFlags = SDL_INIT_EVERYTHING;
	const int sdlInit = SDL_Init(initFlags);
	if(sdlInit == 0){
		initSDL = true;
		Logger::verbose("Initialized SDL.");
	}
	else{
		Logger::errorSDL("Could not initialize SDL.", SDL_GetError());
	}

	// Initializing SDL_image with imgFlags.
	const Uint32 imgFlags = IMG_INIT_PNG;
	if((IMG_Init(imgFlags) & imgFlags)){
		initIMG = true;
		Logger::verbose("Initialized SDL_Image.");
	}
	else{
		Logger::errorSDL("Could not initialize SDL_Image.", IMG_GetError());
	}

	// If even one system fails to initialize, returns false.
	return (initSDL && initIMG);
}

void SDLWrapper::close(){
	Logger::verbose("Closing SDL.");
	// Quits SDL_image.
	IMG_Quit();
	// Quits SDL.
	SDL_Quit();
}
