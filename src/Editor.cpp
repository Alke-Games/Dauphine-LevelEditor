#include "Editor.h"
#include "InputHandler.h"
#include "Configuration.h"
#include "Logger.h"
#include <fstream>
#include <cstdlib>
#include <ctime>

string Editor::mapName = "res/maps/";
int Editor::numberOfTiles = 0;

Editor::Editor() :
	tileSheet(nullptr),
	currentType(TileCode::WHITE),
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

		if(this->currentType > TileCode::GRAY){
			this->currentType = TileCode::BLACK;
		}
	}
	
	// Scroll up through tiles.
	else if(keyStates[GameKeys::UP]){
		this->currentType--;
		
		if(this->currentType < TileCode::BLACK){
			this->currentType = TileCode::GRAY;
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

		if(i == TILE_PER_ROW_IMAGE){
			x = 0;
			y++;
		}

		clips[i].x = TILE_WIDTH * x;
		clips[i].y = TILE_HEIGHT * y;

		clips[i].w = TILE_WIDTH;
		clips[i].h = TILE_HEIGHT;

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
			x += TILE_WIDTH;
		
			//If we've gone too far
			if(x >= LEVEL_WIDTH){
				//Move back
				x = 0;
			
				//Move to the next row
				y += TILE_HEIGHT;    
			}
		}
	}
	else{

		// Discard the number of tiles, on the beggining of the file.
		int dummy = 0;
		map >> dummy;

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
			x += TILE_WIDTH;
		
			//If we've gone too far
			if( x >= LEVEL_WIDTH ){
				//Move back
				x = 0;
			
				//Move to the next row
				y += TILE_HEIGHT;    
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
	if( x < TILE_WIDTH ){
		this->camera.x -= speed;
	}
	
	//Move camera to the right if needed
	if( x > (int)Configuration::getScreenWidth() - TILE_WIDTH ){
		this->camera.x += speed;
	}
	
	//Move camera up if needed
	if( y < TILE_WIDTH ){
		this->camera.y -= speed;
	}
	
	//Move camera down if needed
	if( y > (int)Configuration::getScreenHeight() - TILE_WIDTH ){
		this->camera.y += speed;
	}

	//Keep the camera in bounds.
	if( this->camera.x < 0 ){
		this->camera.x = 0;    
	}
	if( this->camera.y < 0 ){
		this->camera.y = 0;    
	}
	if( this->camera.x > LEVEL_WIDTH - this->camera.w ){
		this->camera.x = LEVEL_WIDTH - this->camera.w;    
	}
	if( this->camera.y > LEVEL_HEIGHT - this->camera.h ){
		this->camera.y = LEVEL_HEIGHT - this->camera.h;    
	} 
}

void Editor::saveTiles(){
	Logger::log("=== Saving tiles...");

	//Open the map
	std::ofstream map(Editor::mapName);
	
	//Go through the tiles
	map << Editor::numberOfTiles << std::endl;
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
	int numberOfTilesRead = 0;
	map >> numberOfTilesRead;
	Editor::numberOfTiles = numberOfTilesRead;
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
