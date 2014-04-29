#include "Sprite.h"
#include "Window.h"
#include "Logger.h"

Sprite::Sprite(const string& path_) :
	sdlTexture(nullptr),
	width(0),
	height(0)
{
	loadFrom(path_);
}

Sprite::~Sprite(){
	free();
}

void Sprite::loadFrom(const string& path_){
	// Warns if loading a sprite without a renderer.
	if(Window::getRenderer() == nullptr){
		Logger::warning("Trying to load sprite with null renderer.");
	}

	// The final texture.
	SDL_Texture* newTexture = nullptr;

	SDL_Surface* loadedSurface = IMG_Load(path_.c_str());
	if(loadedSurface != nullptr){
		//Color key image to magenta.
		//SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0, 0xFF));

		// Create texture from the surface pixels.
        newTexture = SDL_CreateTextureFromSurface(Window::getRenderer(), loadedSurface);
		if(newTexture != nullptr){
			// Set the Sprites width and height, from the loaded surface.
			this->width = loadedSurface->w;
			this->height = loadedSurface->h;
		}
		else{
			Logger::warning("Could not create texture from surface.");
		}

		// Free the loaded surface.
		SDL_FreeSurface(loadedSurface);
	}
	else{
		Logger::warning("Could not load surface from path.");
	}

	// Returns whether the Sprites texture is null or not.
	this->sdlTexture = newTexture;

	// Display error log if image wasn't loaded.
	if(this->sdlTexture == nullptr){
		Logger::error("Sprite load failed: " + path_);
	}
}

void Sprite::free(){
	// If the texture already exists, free it.
	if(this->sdlTexture != nullptr){
		SDL_DestroyTexture(this->sdlTexture);
		this->sdlTexture = nullptr;
		this->width = 0;
		this->height = 0;
	}
	else{
		// Do nothing.
	}
}

void Sprite::render(double x_, double y_, SDL_Rect* clip_, double angle_, SDL_Point* center_,
	SDL_RendererFlip flip_){

	// This is the destination SDL_Rect structure.
	SDL_Rect renderQuad = {(int)x_, (int)y_, (int)this->width, (int)this->height};

	if(clip_ != nullptr){
		renderQuad.w = clip_->w;
		renderQuad.h = clip_->h;
	}
	else{
		// Don't clip the texture.
	}

	int successfullRender = SDL_RenderCopyEx(Window::getRenderer(), this->sdlTexture, clip_,
		&renderQuad, angle_, center_, flip_);
	
	if(successfullRender != 0){
		Logger::errorSDL("Failed to render sprite.", SDL_GetError());
	}
}

unsigned int Sprite::getWidth(){
	return this->width;
}

unsigned int Sprite::getHeight(){
	return this->height;
}
