#ifndef INCLUDE_LOGGER_H
#define INCLUDE_LOGGER_H

#include <string>
using std::string;

/**
* Class used for logging.
* No instances are needed, all the methods are static.
*/
class Logger {

	public:
		/**
		* The default logger.
		* Logs the desired message with a "Log: " prefix.
		* @param message_ : the message to log.
		*/
		static void log(const string& message_);

		/**
		* The logger for warnings.
		* Logs the desired message with a "Warning: " prefix.
		* @param message_ : the message to log.
		*/
		static void warning(const string& message_);

		/**
		* The logger for errors.
		* Logs the desired message with an "Error: " prefix.
		* @param message_ : the message to log.
		*/
		static void error(const string& message_);

		/**
		* The logger for internal SDL errors.
		* Logs the desired message with an "Error: " prefix, accompanied by the error string returned by the GetError() from the SDL libraries.
		* @param message_ : the message to log.
		* @param messageSDL_ : the error string from SDL to log.
		* @see SDL_GetError, IMG_GetError, Mix_GetError
		*/
		static void errorSDL(const string& message_, const char *messageSDL_);

		/**
		* The verbose logger.
		* Logs the desired message with a "Verbose: " prefix.
		* @param message_ : the message to log.
		* @note Activated with the -v flag.
		*/
		static void verbose(const string& message_);

		static bool isVerbose; /**< If the verbose is activated or not. */

};

#endif //INCLUDE_LOGGER_H
