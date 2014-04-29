#ifndef INCLUDE_INPUTHANDLER_H
#define INCLUDE_INPUTHANDLER_H

#include "SDLWrapper.h"

/**
* Handles player input.
* Using SDL event handling, recieves the player input accordingly.
*/
class InputHandler {

	public:
		/**
		* Singleton implementation.
		* @return The InputHandler::instance
		*/
		static InputHandler* getInstance();
		
		/**
		* The destructor.
		* Deletes the InputHandler instance.
		*/
		~InputHandler();

		/**
		* Handles the input.
		* Detects the pending events, and handles them appropriately.
		*/
		void handleInput();

		/**
		* @return If a quit signal was recieved or not.
		*/
		bool signalQuit();

		bool getLmb();
		bool getScrollUp();
		bool getScrollDown();

		void setLmb(bool lmb_);
		void setScrollUp(bool scrollUp_);
		void setScrollDown(bool scrollDown_);

	private:
		/**
		* The constructor.
		* Used to create the input handler instance.
		*/
		InputHandler();

		static InputHandler* instance; /**< The InputHandler instance (Singleton). */

		SDL_Event eventHandler; /**< SDL internal event handler. */
		bool quit; /**< If the quit signal was recieved or not. */
		bool lmb;
		bool scrollUp;
		bool scrollDown;

};

#endif //INCLUDE_INPUTHANDLER_H
