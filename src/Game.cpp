#include "Game.h"
#include "InputHandler.h"
#include "FPSWrapper.h"
#include "Logger.h"
#include "Sprite.h"
#include <fstream>

Game::Game(Window* window_) :
	window(window_),
	isRunning(false)
{
	if(this->window != nullptr){
		this->isRunning = true;
		FPSWrapper::initialize(this->fpsManager);
	}
	else{
		Logger::error("Game window is null. Game will not run.");
	}
}

Game::~Game(){
	if(this->window != nullptr){
		this->window->destroy();
		this->window = nullptr;
	}
}

void Game::clipTiles(SDL_Rect clips[TileCode::TOTAL]){
    //Clip the sprite sheet

    for(int i = 0; i < TileCode::TOTAL; i++){
        if(i < TILE_PER_ROW_IMAGE){
            clips[i].x = 48*i;
            clips[i].y = 0;
        }
        else{
            clips[i].x = 0;
            clips[i].y = 48*i;
        }
        

        clips[i].w = TILE_WIDTH;
        clips[i].h = TILE_HEIGHT;
    }

}

bool Game::setTiles(Tile** tiles){
    //The tile offsets
    int x = 0, y = 0;
    
    //Open the map
    std::ifstream map( "res/lazy.map" );
    
    //If the map couldn't be loaded
    if( map == NULL ){
        //Initialize the tiles
        for( int t = 0; t < TOTAL_TILES; t++ ){
            //Put a floor tile
            tiles[ t ] = new Tile( x, y, t % ( TileCode::RED + 1 ) );
            
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
    }
    else{
        //Initialize the tiles
        for( int t = 0; t < TOTAL_TILES; t++ ){
            //Determines what kind of tile will be made
            int tileType = -1;
    
            //Read tile from map file
            map >> tileType;
        
            //If there was a problem in reading the map
            if( map.fail() == true ){
                //Stop loading map
                map.close();
                return false;
            }
        
            //If the number is a valid tile number
            if( ( tileType >= 0 ) && ( tileType < TileCode::TOTAL ) ){
                tiles[ t ] = new Tile( x, y, tileType );    
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

void Game::setCamera(SDL_Rect& camera){
    //Mouse offsets
    int x = 0, y = 0;
    
    //Get mouse offsets
    SDL_GetMouseState( &x, &y );
    
    //Move camera to the left if needed
    if( x < TILE_WIDTH ){
        camera.x -= 20;
    }
    
    //Move camera to the right if needed
    if( x > (int)Configuration::getScreenWidth() - TILE_WIDTH ){
        camera.x += 20;
    }
    
    //Move camera up if needed
    if( y < TILE_WIDTH ){
        camera.y -= 20;
    }
    
    //Move camera down if needed
    if( y > (int)Configuration::getScreenHeight() - TILE_WIDTH ){
        camera.y += 20;
    }

    //Keep the camera in bounds.
    if( camera.x < 0 ){
        camera.x = 0;    
    }
    if( camera.y < 0 ){
        camera.y = 0;    
    }
    if( camera.x > LEVEL_WIDTH - camera.w ){
        camera.x = LEVEL_WIDTH - camera.w;    
    }
    if( camera.y > LEVEL_HEIGHT - camera.h ){
        camera.y = LEVEL_HEIGHT - camera.h;    
    } 
}

void Game::saveTiles(Tile** tiles){
    Logger::log("saving tiles.");

    //Open the map
    std::ofstream map("res/lazy.map");
    
    //Go through the tiles
    for(int i = 0; i < TOTAL_TILES; i++){
        //Write tile type to file
        map << tiles[i]->getType() << " ";
    }
    
    //Close the file
    map.close();
}

void Game::showType( int tileType_ ){
    switch(tileType_){
        case TileCode::BLACK :
            this->window->setTitle( "Level Designer. Current Tile: BLACK");
            break;
        
        case TileCode::WHITE :
            this->window->setTitle( "Level Designer. Current Tile: WHITE");
            break;
        
        case TileCode::RED :
            this->window->setTitle( "Level Designer. Current Tile: RED");
            break;
        
        case TileCode::GREEN :
            this->window->setTitle( "Level Designer. Current Tile: GREEN");
            break;
        
        case TileCode::BLUE :
            this->window->setTitle( "Level Designer. Current Tile: BLUE");
            break;
        
        case TileCode::YELLOW :
            this->window->setTitle( "Level Designer. Current Tile: YELLOW");
            break;
        
        case TileCode::LIGHTBLUE :
            this->window->setTitle( "Level Designer. Current Tile: LIGHTBLUE");
            break;
        
        case TileCode::PINK :
            this->window->setTitle( "Level Designer. Current Tile: PINK");
            break;
        
        case TileCode::DARKGRAY :
            this->window->setTitle( "Level Designer. Current Tile: DARKGRAY");
            break;
        
        case TileCode::GRAY :
            this->window->setTitle( "Level Designer. Current Tile: GRAY");
            break;

        default:
            this->window->setTitle( "Level Designer. Current Tile: ??????");
            break;

    };    
}

void Game::putTile(Tile** tiles, int tileType, SDL_Rect& camera){
    //Mouse offsets
    int x = 0, y = 0;
    
    //Get mouse offsets
    SDL_GetMouseState( &x, &y );

    Logger::log("mousestate: " + std::to_string(x) + ", " + std::to_string(y));
    
    //Adjust to camera
    x += camera.x;
    y += camera.y;

    Logger::log("mousestate+camera: " + std::to_string(x) + ", " + std::to_string(y));

    
    //Go through tiles
    for( int t = 0; t < TileCode::TOTAL; t++ )
    {
        //Get tile's collision box
        SDL_Rect box = tiles[ t ]->getRectangle();
        
        //If the mouse is inside the tile
        if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
        {
            Logger::log("old type: " + std::to_string(tiles[t]->getType()));
            //Get rid of old tile
            delete tiles[ t ];
            
            //Replace it with new one
            tiles[ t ] = new Tile( box.x, box.y, tileType );

            Logger::log("new type: " + std::to_string(tiles[t]->getType()));
        }
    }
}

void Game::runGame(){
    
    // Creating the input handler.
    InputHandler* inputHandler = InputHandler::getInstance();

    // Get the first game time.
    double totalGameTime = 0.0;
    const double deltaTime = 1.0 / 60.0;
    double accumulatedTime = 0.0;

    // Level Editor code.
    Sprite* tileSheet = new Sprite("res/tilesheet.png");
    SDL_Rect clips[TileCode::TOTAL];
    Tile* tiles[TOTAL_TILES];
    Tile::tileSheet = tileSheet;
    SDL_Rect camera = {(int)0, (int)0, (int)Configuration::getScreenWidth(), (int)Configuration::getScreenHeight()};
    clipTiles(clips);
    setTiles(tiles);
    int currentType = TileCode::BLACK;
    showType(currentType);
    double countDelay = 1.0;
    const double minimumDelay = 0.1;

    // This is the main game loop.
    while(this->isRunning){

        const double frameTime = FPSWrapper::delay(this->fpsManager);
        accumulatedTime += frameTime;

        // Update.
        while(accumulatedTime >= deltaTime){
            inputHandler->handleInput();

            if(totalGameTime >= countDelay + minimumDelay){
                countDelay = totalGameTime;
                //Logger::log("totalGameTime: " + std::to_string(totalGameTime) + " countDelay: " + std::to_string(countDelay));

                if(inputHandler->getLmb()){
                    putTile(tiles, currentType, camera);
                    Logger::log("MOUSE CLICK LMB");
                    inputHandler->setLmb(false);
                }

                if(inputHandler->getScrollDown()){
                    Logger::log("MOUSE SCROLL DOWN");
                    //Scroll through tiles
                    currentType++;

                    if( currentType > TileCode::GRAY ){
                        currentType = TileCode::BLACK;
                    }

                    //Show the current tile type
                    showType(currentType);
                    inputHandler->setScrollDown(false);
                }

                if(inputHandler->getScrollUp()){
                    Logger::log("MOUSE SCROLL UP");
                    //Scroll through tiles
                    currentType--;
                    
                    if( currentType < TileCode::BLACK ){
                        currentType = TileCode::GRAY;
                    }
                    
                    //Show the current tile type
                    showType(currentType);
                    inputHandler->setScrollUp(false);
                }
            }

            


            // Check for a quit signal from input.
            if(inputHandler->signalQuit()){
                this->isRunning = false;
                break;
            }

            //Game::currentState->update(deltaTime);

            accumulatedTime -= deltaTime;
            totalGameTime += deltaTime;
        }

        setCamera(camera);

        // Render.
        window->clear();

        //Game::currentState->render();
        //Show the tiles
        for(int i = 0; i < TOTAL_TILES; i++ ){
            tiles[i]->render(camera, clips);
        }

        window->render();
        
    }

    saveTiles(tiles);

    //Free the surface
    delete Tile::tileSheet;
    
    //Free the tiles
    for(int i = 0; i < TOTAL_TILES; i++){
        delete tiles[i];    
    }

}
