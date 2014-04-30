#include "Window.h"
#include "Logger.h"
#include "Configuration.h"


SDL_Renderer* Window::sdlRenderer = nullptr;


Window::Window(const unsigned int width_, const unsigned int height_, const string& title_) :
	windowTitle(title_),
	sdlWindow(nullptr)
{
	create(width_, height_);
}

Window::~Window(){
	destroy();
}

void Window::destroy(){
	// Destroys the Window renderer.
	SDL_DestroyRenderer(Window::sdlRenderer);
	Window::sdlRenderer = nullptr;

	// Destroys the Window window.
	SDL_DestroyWindow(this->sdlWindow);
	this->sdlWindow = nullptr;
}

void Window::minimize(){
	SDL_MinimizeWindow(this->sdlWindow);
}

void Window::maximize(){
	SDL_MaximizeWindow(this->sdlWindow);
}

void Window::clear(){
	SDL_SetRenderDrawColor(Window::sdlRenderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(Window::sdlRenderer);
}

void Window::render(){
	SDL_RenderPresent(Window::sdlRenderer);
}

void Window::setTitle(const string& title_){
	SDL_SetWindowTitle(this->sdlWindow, title_.c_str());
}

void Window::create(const unsigned int width_, const unsigned int height_){
	/// @todo Toggle VSync.

	// Creates the SDL window.
	const Uint32 windowFlags = SDL_WINDOW_SHOWN;
	this->sdlWindow = SDL_CreateWindow(
		this->windowTitle.c_str(), 
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width_, height_,
		windowFlags
	);
	
	if(this->sdlWindow != nullptr){

		// Creates the SDL renderer.
		Window::sdlRenderer = SDL_CreateRenderer(this->sdlWindow, -1, SDL_RENDERER_ACCELERATED);
		if(Window::sdlRenderer != nullptr){

			// Set texture filtering to linear.
			SDL_bool linearFilter = SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
			if(linearFilter){
				Logger::log("Linear texture filtering enabled!");
				/// @todo Fix mouse x,y with rescale.
				//rescale(Configuration::getLogicalRenderSize());
			}
			else{
				Logger::warning("Linear texture filtering disabled!");
			}

			Logger::verbose("Game successfully loaded.");
				
		}
		else{
			Logger::errorSDL("Renderer could not be created.", SDL_GetError());
		}
	}
	else{
		Logger::errorSDL("Window failed to be created.", SDL_GetError());
	}
}

void Window::resize(const unsigned int width_, const unsigned int height_){
	SDL_SetWindowSize(this->sdlWindow, width_, height_);
}

void Window::rescale(unsigned int size_){
	// Just a precaution, so there is no abuse on the size.
	if(size_ > 10){
		size_ = 10;
		Logger::warning("Trying to rescale for a value too big.");
	}

	SDL_RenderSetLogicalSize(
		Window::sdlRenderer,
		Configuration::getResolutionWidth() * size_,
		Configuration::getResolutionHeight() * size_
	);
}

SDL_Renderer* Window::getRenderer(){
	return Window::sdlRenderer;
}
