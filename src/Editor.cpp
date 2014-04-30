#include "Editor.h"
#include "InputHandler.h"
#include "Configuration.h"
#include "Logger.h"
#include <fstream>

string Editor::mapName = "res/maps/";
int Editor::numberOfTiles = 0;

Editor::Editor() :
	tileSheet(nullptr),
	currentType(TileCode::BLACK),
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
		if(!setTiles()){
			Logger::warning("Could not set tiles.");
		}
		showType();
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

	if(keyStates[GameKeys::LMB_DOWN]){
		putTile();
	}

	if(keyStates[GameKeys::DOWN]){
		//Scroll through tiles
		this->currentType++;

		if(this->currentType > TileCode::GRAY){
			this->currentType = TileCode::BLACK;
		}

		// //Show the current tile type
		showType();
	}
	

	else if(keyStates[GameKeys::UP]){
		//Scroll through tiles
		this->currentType--;
		
		if(this->currentType < TileCode::BLACK){
			this->currentType = TileCode::GRAY;
		}
		
		//Show the current tile type
		showType();
	}

}

void Editor::render(){
	setCamera();
	for(int i = 0; i < Editor::numberOfTiles; i++ ){
		const int dx = tiles[i]->getRectangle().x - camera.x;
		const int dy = tiles[i]->getRectangle().y - camera.y;
		this->tileSheet->render(dx, dy, &clips[tiles[i]->getType()]);
	}
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
	//The tile offsets
	int x = 0, y = 0;
	
	//Open the map
	// std::ifstream map( "res/lazy.map" );
	std::ifstream map(Editor::mapName);
	
	//If the map couldn't be loaded
	if(map == nullptr){
		//Initialize the tiles
		for(int t = 0; t < Editor::numberOfTiles; t++){
			//Put a floor tile
			this->tiles[ t ] = new Tile(x, y, t % ( TileCode::RED + 1 ));
			
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

		int dummy = 0;
		map >> dummy;

		//Initialize the tiles
		for(int t = 0; t < Editor::numberOfTiles;t++){
			//Determines what kind of tile will be made
			int tileType = -1;
	
			//Read tile from map file
			map >> tileType;
		
			//If there was a problem in reading the map
			if(map.fail() == true){
				//Stop loading map
				map.close();
				return false;
			}
		
			//If the number is a valid tile number
			if(( tileType >= 0 ) && ( tileType < TileCode::TOTAL )){
				this->tiles[t] = new Tile(x, y, tileType);    
			}
			//If we don't recognize the tile type
			else{
				//Stop loading map
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
	
	//Move camera to the left if needed
	if( x < TILE_WIDTH ){
		this->camera.x -= 20;
	}
	
	//Move camera to the right if needed
	if( x > (int)Configuration::getScreenWidth() - TILE_WIDTH ){
		this->camera.x += 20;
	}
	
	//Move camera up if needed
	if( y < TILE_WIDTH ){
		this->camera.y -= 20;
	}
	
	//Move camera down if needed
	if( y > (int)Configuration::getScreenHeight() - TILE_WIDTH ){
		this->camera.y += 20;
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
	//std::ofstream map("res/lazy.map");
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

void Editor::showType(){
	Logger::log("!!! CURRENT TILE TYPE: " + std::to_string(this->currentType));
	// switch(this->currentType){
	// 	case TileCode::BLACK :
	// 		this->window->setTitle( "Level Designer. Current Tile: BLACK");
	// 		break;
	   
}

void Editor::putTile(){
	//Mouse offsets
	int x = 0, y = 0;
	
	//Get mouse offsets
	SDL_GetMouseState(&x, &y);

	//Logger::verbose("mousestate: " + std::to_string(x) + ", " + std::to_string(y));
	
	//Adjust to camerathis->
	x += this->camera.x;
	y += this->camera.y;

	Logger::verbose("mouse: " + std::to_string(x) + ", " + std::to_string(y));

	//Go through tiles
	for(int t = 0; t < Editor::numberOfTiles; t++){
		//Get tile's collision box
		SDL_Rect box = this->tiles[t]->getRectangle();
		
		//If the mouse is inside the tile
		if( (x > box.x) && (x < box.x + box.w) && (y > box.y) && (y < box.y + box.h) ){
			//cout << "box[" << t << "] = {" << box.x << " " << box.y << " " << box.w << " " << box.h << "}" << endl;
			Logger::verbose("old type: " + std::to_string(this->tiles[t]->getType()));
			//Get rid of old tile
			delete this->tiles[t];
			
			//Replace it with new one
			this->tiles[t] = new Tile(box.x, box.y, this->currentType);

			Logger::verbose("new type: " + std::to_string(this->tiles[t]->getType()));
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
