#include "Logger.h"
#include <iostream>

using std::cerr;
using std::endl;

bool Logger::isVerbose = false;

void Logger::log(const string& message_){
	/// @todo Implement a separate generic parameter log function, to log an integer, for example.
	cerr << "[Log]:     " << message_ << endl;
}

void Logger::warning(const string& message_){
	cerr << "[Warning]: " << message_ << endl;
}

void Logger::error(const string& message_){
	cerr << "[Error]:   " << message_ << endl;
}

void Logger::errorSDL(const string& message_, const char *messageSDL_){
	cerr << "[Error]:   " << message_ << endl;
	cerr << "   [SDL Error]: " << std::string(messageSDL_) << endl;
}

void Logger::verbose(const string& message_){
	if(Logger::isVerbose){
		cerr << "[Verbose]: " << message_ << endl;
	}
}
