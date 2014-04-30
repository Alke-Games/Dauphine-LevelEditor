#include "Main.h"

#include "Game.h"
#include "Editor.h"
#include "Logger.h"
#include "Configuration.h"

#include <iostream>
#include <cstdlib> 
using std::cout;
using std::endl;

void help(){
	cout << "Dauphine Level Editor:" << endl << endl;
	cout << "Usage: ./DauphineLevelEditor [--help] [--verbose] [--new <mapname> <numbertiles>] [--load <mapname>]" << endl << endl;
	cout << " verbose: " << TAB << "Activates the verbose logger." << endl;
	cout << " new: " << TAB << TAB << "Creates a map with <mapname> name, and <numbertiles> tiles." << endl;
	cout << " load: " << TAB << TAB << "Loads an already existing map with <mapname> name." << endl << endl;
	cout << "You must either create or load a map." << endl;
}

int main(int argc, char** argv){
	// if(argc == 2 && argv[1] == std::string("-v")){
	// 	Logger::isVerbose = true;
	// }
	bool correctArguments = false;

	if(argc >= 1 && argc <= 5){
		int i = 0;
		while(argv[i] != nullptr){
			//Logger::log(string(argv[i]));
			if((argv[i] == string("--help")) || argc == 1){
				help();
				return 0;
			}
			else if(argv[i] == string("--verbose")){
				Logger::isVerbose = true;
			}
			else if(argv[i] == string("--new") && (argc == 4 || argc == 5) ){
				Editor::mapName.append(argv[i+1]);
				Editor::numberOfTiles = atoi(argv[i+2]);
				correctArguments = true;
				break;
			}
			else if(argv[i] == string("--load") && (argc == 3 || argc == 4) ){
				Editor::mapName.append(argv[i+1]);
				correctArguments = true;
				break;
			}
			argv[i]++;
			i++;
		}
	}
	else{
		Logger::error("Correct usage: ./DauphineLevelEditor [--help] [--verbose] [--new <mapname> <numbertiles>] [--load <mapname>]\n\nSee './DauphineLevelEditor --help'.");
		return 1;
	}

	if(!correctArguments){
		Logger::error("\nCorrect usage: ./DauphineLevelEditor [--help] [--verbose] [--new <mapname> <numbertiles>] [--load <mapname>]\n\nSee './DauphineLevelEditor --help'.");
		return 1;
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
