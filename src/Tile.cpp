#include "Tile.h"
#include "Logger.h"

Tile::Tile(int x_, int y_, int tileType_) :
	type(tileType_)
{
	this->rectangle = {x_, y_, TILE_WIDTH, TILE_HEIGHT};
}

Tile::~Tile(){

}

SDL_Rect& Tile::getRectangle(){
	return this->rectangle;
}

int Tile::getType(){
	return this->type;
}

// void Tile::render(SDL_Rect& clipCamera, SDL_Rect clips[TileCode::TOTAL]){
//     // if(Tile::tileSheet != nullptr){
//     //---->     //if(checkCollision(clipCamera, this->rectangle)){
//     //      const int dx = this->rectangle.x - clipCamera.x;
//     //      const int dy = this->rectangle.y - clipCamera.y;
//     //      Tile::tileSheet->render(dx, dy, &clips[this->type]);
//     //---->     //}
//     // }
//     Logger::log("rendering tiles??");
// }

// bool Tile::checkCollision( SDL_Rect &A, SDL_Rect &B ){
//     //The sides of the rectangles
//     int leftA, leftB;
//     int rightA, rightB;
//     int topA, topB;
//     int bottomA, bottomB;

//     //Calculate the sides of rect A
//     leftA = A.x;
//     rightA = A.x + A.w;
//     topA = A.y;
//     bottomA = A.y + A.h;
        
//     //Calculate the sides of rect B
//     leftB = B.x;
//     rightB = B.x + B.w;
//     topB = B.y;
//     bottomB = B.y + B.h;
            
//     //If any of the sides from A are outside of B
//     if( bottomA <= topB ){
//         return false;
//     }
//     if( topA >= bottomB ){
//         return false;
//     }
//     if( rightA <= leftB ){
//         return false;
//     }
//     if( leftA >= rightB ){
//         return false;
//     }
    
//     //If none of the sides from A are outside B
//     return true;
// }

