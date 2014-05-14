#include "Main.h"

#include "Game.h"
#include "Editor.h"
#include "Logger.h"
#include "Configuration.h"

#include <iostream>
#include <cstdlib>
#include <fstream>
using std::cout;
using std::endl;

int main(int argc, char** argv){
	bool handledArguments = handleArguments(argc, argv);
	if(!handledArguments){
		return -1;
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

void help(){
	cout << "Dauphine Level Editor:" << endl << endl;
	cout << "Usage: ./DauphineLevelEditor [--help] [--verbose] [--new <mapname> <numbertiles>] [--load <mapname>]" << endl << endl;
	cout << " verbose: " << TAB << "Activates the verbose logger." << endl;
	cout << " new: " << TAB << TAB << "Creates a map with <mapname> name, and <numbertiles> tiles." << endl;
	cout << " load: " << TAB << TAB << "Loads an already existing map with <mapname> name." << endl << endl;
	cout << "You must either create or load a map." << endl;
}

bool handleArguments(int& argc, char**& argv){
	bool correctArguments = false;
	if(argc >= 1 && argc <= 5){
		int i = 0;
		while(argv[i] != nullptr){
			if((argv[i] == string("--help")) || argc == 1){
				help();
				return false;
			}
			else if(argv[i] == string("--verbose")){
				Logger::isVerbose = true;
			}
			else if(argv[i] == string("--new") && (argc == 5 || argc == 6) ){
				Editor::mapName.append(argv[i+1]);
				Editor::LEVEL_WIDTH = atoi(argv[i+2]);
				Editor::LEVEL_HEIGHT = atoi(argv[i+3]);
				Editor::numberOfTiles = (Editor::LEVEL_WIDTH * Editor::LEVEL_HEIGHT) / TILE_SIZE;
				correctArguments = true;
			}
			else if(argv[i] == string("--load") && (argc == 3 || argc == 4) ){
				Editor::mapName.append(argv[i+1]);
				std::ifstream map(Editor::mapName);
				if(map == nullptr){
					Logger::error("Map does not exist, couldn't load.");
					return false;
				}
				correctArguments = true;
			}
			argv[i]++;
			i++;
		}
	}
	else{
		Logger::error("Correct usage: ./DauphineLevelEditor [--help] [--verbose] [--new <mapname> <numbertiles>] [--load <mapname>]\n\nSee './DauphineLevelEditor --help'.");
		return false;
	}
	return correctArguments;
}
