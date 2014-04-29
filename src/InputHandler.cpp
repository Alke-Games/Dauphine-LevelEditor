#include "InputHandler.h"
#include "Logger.h"

InputHandler* InputHandler::instance = nullptr;

InputHandler::InputHandler() :
	quit(false),
	lmb(false),
	scrollUp(false),
	scrollDown(false)
{

}

InputHandler* InputHandler::getInstance(){
	if(InputHandler::instance == nullptr){
		InputHandler::instance =  new InputHandler();
	}

	return InputHandler::instance;
}

InputHandler::~InputHandler(){
	delete InputHandler::instance;
}

void InputHandler::handleInput(){

	int pendingEvent = 0;

	do{
		pendingEvent = SDL_PollEvent(&this->eventHandler); 
		
		//When the user clicks
        if(this->eventHandler.type == SDL_MOUSEBUTTONDOWN){
            //Logger::log("MOUSE CLICK (LMB)");
            //On left mouse click
            if( this->eventHandler.button.button == SDL_BUTTON_LEFT ){
                //Put the tile
                //putTile(tiles, currentType, camera);
            	this->lmb = true;
            }
            else{
            	this->lmb = false;
            }
        }

        //On mouse wheel scroll
        if(this->eventHandler.type == SDL_MOUSEWHEEL){
            //Logger::log("MOUSE SCROLL");
            Sint32 yScrolled = this->eventHandler.button.y;
            if(yScrolled > 0){
                //Scroll through tiles
                // currentType--;
                
                // if( currentType < TileCode::BLACK ){
                //     currentType = TileCode::GRAY;
                // }
                
                // //Show the current tile type
                // showType(currentType);
                this->scrollUp = true;
                this->scrollDown = false;


            }
            else if(yScrolled < 0){
                //Scroll through tiles
                // currentType++;

                // if( currentType > TileCode::GRAY ){
                //     currentType = TileCode::BLACK;
                // }

                // //Show the current tile type
                // showType(currentType);
                this->scrollUp = false;
                this->scrollDown = true;
            }
            else{
                //Logger::warning("no scroll.");
                this->scrollUp = false;
                this->scrollDown = false;
            }

            this->eventHandler.button.y = 0;

        }

		//On window exit (X).
		if(this->eventHandler.type == SDL_QUIT){
	    	this->quit = true;
	    }

	} while(pendingEvent != 0);
}

bool InputHandler::signalQuit(){
	return this->quit;
}

bool InputHandler::getLmb(){
	return this->lmb;
}

bool InputHandler::getScrollUp(){
	return this->scrollUp;
}

bool InputHandler::getScrollDown(){
	return this->scrollDown;
}

void InputHandler::setLmb(bool lmb_){
	this->lmb = lmb_;
}

void InputHandler::setScrollUp(bool scrollUp_){
	this->scrollUp = scrollUp_;
}

void InputHandler::setScrollDown(bool scrollDown_){
	this->scrollDown = scrollDown_;
}

