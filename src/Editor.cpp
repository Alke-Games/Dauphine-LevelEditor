#include "Editor.h"
#include "InputHandler.h"
#include "Configuration.h"
#include "Logger.h"
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <iostream>

string Editor::mapName = "res/maps/";
int Editor::numberOfTiles = 0;
int Editor::LEVEL_WIDTH = 0;
int Editor::LEVEL_HEIGHT = 0;

Editor::Editor() :
	tileSheet(nullptr),
	currentType(TileCode::TOTAL-1),
	camera{(int)0, (int)0, (int)Configuration::getScreenWidth(), (int)Configuration::getScreenHeight()}
{
	this->tileSheet = new Sprite("res/tilesheet.png");

	std::ifstream map(Editor::mapName);
	if(map != nullptr){
		countTiles();
	}

	this->tiles = new Tile*[Editor::numberOfTiles];
	if(this->tileSheet != nullptr){

		clipTiles();
		bool tilesSet = setTiles();
		if(!tilesSet){
			Logger::error("Could not set tiles.");
		}

	}
	else{
		Logger::warning("TileSheet image could not be loaded in Editor!");
	}
}

Editor::~Editor(){
	saveTiles();

	//Free the sprite
	delete this->tileSheet;
	
	//Free the tiles
	for(int i = 0; i < Editor::numberOfTiles; i++){
		delete tiles[i];    
	}

	delete tiles;

}

void Editor::update(){
	InputHandler* inputHandler = InputHandler::getInstance();
	array<bool, GameKeys::MAX> keyStates = inputHandler->getKeyStates();

	// Put the current tile on cursor position.
	if(keyStates[GameKeys::LMB_DOWN]){
		putTile();
	}

	// Scroll down through tiles.
	if(keyStates[GameKeys::DOWN]){
		this->currentType++;

		if(this->currentType > TileCode::TOTAL-1){
			this->currentType = 0;
		}
	}
	
	// Scroll up through tiles.
	else if(keyStates[GameKeys::UP]){
		this->currentType--;
		
		if(this->currentType < 0){
			this->currentType = TileCode::TOTAL-1;
		}
	}

}

void Editor::render(){
	// Move the camera.
	setCamera();

	// Render all the tiles.
	for(int i = 0; i < Editor::numberOfTiles; i++ ){
		if(checkCollision(camera, tiles[i]->getRectangle())){
			const int dx = tiles[i]->getRectangle().x - camera.x;
			const int dy = tiles[i]->getRectangle().y - camera.y;
			this->tileSheet->render(dx, dy, &clips[tiles[i]->getType()]);
		}
	}

	renderCurrentTileOnCursor();
}




///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////





void Editor::clipTiles(){
	//Clip the sprite sheet
	int x = 0;
	int y = 0;
	for(int i = 0; i < TileCode::TOTAL; i++){

		if(i!=0 && i%TILE_PER_ROW_IMAGE == 0){
			x = 0;
			y++;
		}

		clips[i].x = TILE_SIZE * x;
		clips[i].y = TILE_SIZE * y;

		clips[i].w = TILE_SIZE;
		clips[i].h = TILE_SIZE;

		x++;

	}
}

bool Editor::setTiles(){
	// The tile offsets.
	int x = 0, y = 0;
	
	// Open the map.
	std::ifstream map(Editor::mapName);
	
	// If the map does not exist, create one.
	if(map == nullptr){
		srand(time(nullptr));

		// Initialize the tiles.
		for(int t = 0; t < Editor::numberOfTiles; t++){
			//Put a floor tile
			int randomTileType = rand() % (TileCode::TOTAL);
			this->tiles[ t ] = new Tile(x, y, randomTileType);
			
			//Move to next tile spot
			x += TILE_SIZE;
		
			//If we've gone too far
			if(x >= LEVEL_WIDTH){
				//Move back
				x = 0;
			
				//Move to the next row
				y += TILE_SIZE;    
			}
		}
	}
	else{

		// Discard the width/height, on the beggining of the file.
		int dummywidth = 0;
		int dummyheight = 0;
		map >> dummywidth >> dummyheight;

		//Initialize the tiles
		for(int t = 0; t < Editor::numberOfTiles;t++){

			// Determines what kind of tile will be made.
			int tileType = -1;
	
			//Read tile from map file
			map >> tileType;
		
			//If there was a problem in reading the map
			if(map.fail()){
				//Stop loading map
				map.close();
				return false;
			}
		
			// If the number is a valid tile type.
			if( (tileType >= 0) && (tileType < TileCode::TOTAL) ){
				this->tiles[t] = new Tile(x, y, tileType);    
			}
			else{
				Logger::warning("Tile type not recognized on loading.");
				map.close();
				return false;
			}
		
			//Move to next tile spot
			x += TILE_SIZE;
		
			//If we've gone too far
			if( x >= LEVEL_WIDTH ){
				//Move back
				x = 0;
			
				//Move to the next row
				y += TILE_SIZE;    
			}
		}
	
		//Close the file
		map.close();
	}
	
	
	return true;
}

void Editor::setCamera(){
	//Mouse offsets
	int x = 0, y = 0;
	
	//Get mouse offsets
	SDL_GetMouseState( &x, &y );

	const int speed = 5;
	
	//Move camera to the left if needed
	if( x < TILE_SIZE ){
		this->camera.x -= speed;
	}
	
	//Move camera to the right if needed
	if( x > (int)Configuration::getScreenWidth() - TILE_SIZE ){
		this->camera.x += speed;
	}
	
	//Move camera up if needed
	if( y < TILE_SIZE ){
		this->camera.y -= speed;
	}
	
	//Move camera down if needed
	if( y > (int)Configuration::getScreenHeight() - TILE_SIZE ){
		this->camera.y += speed;
	}

	//Keep the camera in bounds.
	if( this->camera.x < 0 ){
		this->camera.x = 0;    
	}
	if( this->camera.y < 0 ){
		this->camera.y = 0;    
	}
	if( this->camera.x > Editor::LEVEL_WIDTH - this->camera.w ){
		this->camera.x = Editor::LEVEL_WIDTH - this->camera.w;    
	}
	if( this->camera.y > Editor::LEVEL_HEIGHT - this->camera.h ){
		this->camera.y = Editor::LEVEL_HEIGHT - this->camera.h;    
	} 
}

void Editor::saveTiles(){
	Logger::log("=== Saving tiles...");

	//Open the map
	std::ofstream map(Editor::mapName);
	
	//Go through the tiles
	map << Editor::LEVEL_WIDTH << std::endl;
	map << Editor::LEVEL_HEIGHT << std::endl;
	for(int i = 0; i < Editor::numberOfTiles; i++){

		//Write tile type to file
		map << this->tiles[i]->getType() << " ";
	}
	
	//Close the file
	map.close();
	Logger::log("=== Done saving tiles");
}

void Editor::putTile(){
	//Mouse offsets
	int x = 0, y = 0;
	
	//Get mouse offsets
	SDL_GetMouseState(&x, &y);

	// Adjust to camera position.
	x += this->camera.x;
	y += this->camera.y;

	//Go through tiles
	for(int t = 0; t < Editor::numberOfTiles; t++){

		//Get tile's collision box
		SDL_Rect box = this->tiles[t]->getRectangle();
		
		//If the mouse is inside the tile
		if( (x > box.x) && (x < box.x + box.w) && (y > box.y) && (y < box.y + box.h) ){
			
			//Get rid of old tile
			delete this->tiles[t];
			
			//Replace it with new one
			this->tiles[t] = new Tile(box.x, box.y, this->currentType);
		}
	}
}

void Editor::renderCurrentTileOnCursor(){
	//Mouse offsets
	int x = 0, y = 0;
	
	//Get mouse offsets
	SDL_GetMouseState(&x, &y);

	
	// Adjust to camera position.
	x += this->camera.x;
	y += this->camera.y;

	//Go through tiles
	for(int t = 0; t < Editor::numberOfTiles; t++){
		//Get tile's collision box
		SDL_Rect box = this->tiles[t]->getRectangle();
		
		//If the mouse is inside the tile
		if( (x > box.x) && (x < box.x + box.w) && (y > box.y) && (y < box.y + box.h) ){
			const int dx = tiles[t]->getRectangle().x - camera.x;
			const int dy = tiles[t]->getRectangle().y - camera.y;
			this->tileSheet->render(dx, dy, &clips[this->currentType]);
		}
	}
}

void Editor::countTiles(){
	std::ifstream map(Editor::mapName);
	int width = 0;
	int height = 0;
	map >> width;
	map >> height;
	Editor::LEVEL_WIDTH = width;
	Editor::LEVEL_HEIGHT = height;
	Editor::numberOfTiles = (Editor::LEVEL_WIDTH * Editor::LEVEL_HEIGHT) / TILE_SIZE;
	std::cout << "w: " << Editor::LEVEL_WIDTH << " h: " << Editor::LEVEL_HEIGHT << " n: " << Editor::numberOfTiles << std::endl;
	map.close();
}

bool Editor::checkCollision( SDL_Rect &A, SDL_Rect &B ){
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = A.x;
    rightA = A.x + A.w;
    topA = A.y;
    bottomA = A.y + A.h;
        
    //Calculate the sides of rect B
    leftB = B.x;
    rightB = B.x + B.w;
    topB = B.y;
    bottomB = B.y + B.h;
            
    //If any of the sides from A are outside of B
    if( bottomA <= topB ){
        return false;
    }
    if( topA >= bottomB ){
        return false;
    }
    if( rightA <= leftB ){
        return false;
    }
    if( leftA >= rightB ){
        return false;
    }
    
    //If none of the sides from A are outside B
    return true;
}
