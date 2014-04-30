#ifndef INCLUDE_INPUTHANDLER_H
#define INCLUDE_INPUTHANDLER_H

#include "SDLWrapper.h"
#include "InputKeys.h"

#include <array>
using std::array;

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
		bool handleInput();

		/**
		* @return The keyStates attribute.
		*/
		array<bool, GameKeys::MAX> getKeyStates();

		/**
		* @return If a quit signal was recieved or not.
		*/
		bool signalQuit();

	private:
		/**
		* The constructor.
		* Used to create the input handler instance.
		*/
		InputHandler();

		static InputHandler* instance;

		array<bool, GameKeys::MAX> keyStates; /**< Boolean array that controls which keys are
			pressed or not. */
		SDL_Event eventHandler; /**< SDL internal event handler. */
		bool quit; /**< If the quit signal was recieved or not. */

};

#endif //INCLUDE_INPUTHANDLER_H
