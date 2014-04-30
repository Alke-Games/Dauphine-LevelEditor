#include "Game.h"
#include "InputHandler.h"
#include "FPSWrapper.h"
#include "Editor.h"
#include "Logger.h"


Game::Game(Window* window_) :
	window(window_),
	isRunning(false)
{
	if(this->window != nullptr){
		this->isRunning = true;
		FPSWrapper::initialize(this->fpsManager);
	}
	else{
		Logger::error("Game window is null. Game will not run.");
	}
}

Game::~Game(){
	if(this->window != nullptr){
		this->window->destroy();
		this->window = nullptr;
	}
}

void Game::runGame(){
    
    // Creating the input handler.
    InputHandler* inputHandler = InputHandler::getInstance();

    Editor editor;

    // This is the main game loop.
    while(this->isRunning){

        // Update.
        while(inputHandler->handleInput()){
            // Check for a quit signal from input.
            if(inputHandler->signalQuit()){
                this->isRunning = false;
                break;
            }

            editor.update();
        }

        // Render.
        window->clear();

        editor.render();        

        window->render();
        
    }    

}
