#include "Game.h"
#include "Logger.h"
#include "Configuration.h"

int main(int argc, char** argv){
	if(argc == 2 && argv[1] == std::string("-v")){
		Logger::isVerbose = true;
	}

	Logger::verbose("Starting Dauphine Level Editor...");

	Configuration::initialize();

	bool systemsInitialized = SDLWrapper::initialize();

	if(systemsInitialized){
		Window window(Configuration::getScreenWidth(), Configuration::getScreenHeight(),
			Configuration::getWindowTitle());

		Game* game = nullptr;
		game = new Game(&window);

		game->runGame();

		if(game != nullptr){
			delete game;
		}
	}
	else{
		Logger::error("Systems were not initialized.");
	}

	SDLWrapper::close();

	Logger::verbose("Exiting Dauphine Level Editor...");
	return 0;
}
