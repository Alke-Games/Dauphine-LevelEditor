#include "InputHandler.h"
#include "Logger.h"


InputHandler* InputHandler::instance = nullptr;


InputHandler::InputHandler() :
	quit(false)
{
	for(unsigned int i = 0; i < GameKeys::MAX; i++){
		this->keyStates[i] = false;
	}
}

InputHandler* InputHandler::getInstance(){
	if(InputHandler::instance == nullptr){
		InputHandler::instance = new InputHandler();
	}
	return InputHandler::instance;
}

InputHandler::~InputHandler(){
	
}

bool InputHandler::handleInput(){

	int pendingEvent = SDL_PollEvent(&this->eventHandler); 

	// On keydown.
	if(this->eventHandler.type == SDL_KEYDOWN){
		switch(this->eventHandler.key.keysym.sym){
			case SDLK_UP: // UP.
				this->keyStates[GameKeys::UP] = true;
				break;
			case SDLK_LEFT: // LEFT.
				this->keyStates[GameKeys::LEFT] = true;
				break;
			case SDLK_RIGHT: // RIGHT.
				this->keyStates[GameKeys::RIGHT] = true;
				break;
			case SDLK_DOWN: // DOWN.
				this->keyStates[GameKeys::DOWN] = true;
				break;
			default:
				break;
		}
	}

	// On keyup.
	else if(this->eventHandler.type == SDL_KEYUP){
		switch(this->eventHandler.key.keysym.sym){
			case SDLK_UP: // UP.
				this->keyStates[GameKeys::UP] = false;
				break;
			case SDLK_LEFT: // LEFT.
				this->keyStates[GameKeys::LEFT] = false;
				break;
			case SDLK_RIGHT: // RIGHT.
				this->keyStates[GameKeys::RIGHT] = false;
				break;
			case SDLK_DOWN: // DOWN.
				this->keyStates[GameKeys::DOWN] = false;
				break;
			default:
				break;
		}
	}

	// On mouse down.
	else if(this->eventHandler.type == SDL_MOUSEBUTTONDOWN){
		// LMB.
		if(this->eventHandler.button.button == SDL_BUTTON_LEFT){
			this->keyStates[GameKeys::LMB_DOWN] = true;
		}
	}
	
	// On mouse up.
	else{
		this->keyStates[GameKeys::LMB_DOWN] = false;
	}
	
	//On window exit (X).
	if(this->eventHandler.type == SDL_QUIT){
		this->quit = true;
	}

	return (pendingEvent == 0) ? false : true;
}

array<bool, GameKeys::MAX> InputHandler::getKeyStates(){
	return this->keyStates;
}

bool InputHandler::signalQuit(){
	return this->quit;
}
