/* Code by: Group 16
 * Date: October 25, 2019
 */

#include "playerTexture.h"

/**
 * constructor for playerTexture
 */
playerTexture::playerTexture(){
	gameTexture = nullptr;
	txtHeight = 0;
	txtWidth = 0;
	
}

/**
 * destructor
 */
playerTexture::~playerTexture(){
	free();
}

/**
 * frees memory
 */
void playerTexture::free(){
	
	if(gameTexture != nullptr){
		SDL_DestroyTexture(gameTexture);
		gameTexture = nullptr;
		txtHeight = 0;
		txtWidth = 0;
	}
}

/**
 * loads images for player
 * @param image - image for player
 * @param rend
 * @return
 */
int playerTexture::loadImage(std::string image, SDL_Renderer*& rend){
	
	free();
	SDL_Texture* newTexture;
	SDL_Surface* toLoad = IMG_Load(image.c_str());
	if(toLoad == nullptr){
		return(-1);
	}
	
	SDL_SetColorKey(toLoad, SDL_TRUE, SDL_MapRGB((*toLoad).format, 0, 0xFF, 0xFF));
	
	newTexture = SDL_CreateTextureFromSurface(rend, toLoad);
	if(newTexture == nullptr){
		return(-2);
	}
	
	txtWidth = (*toLoad).w;
	txtHeight = (*toLoad).h;
	
	SDL_FreeSurface(toLoad);
	
	gameTexture = newTexture;
	return 0;
	
}

/**
 *
 * @param x - x coordinate of player texture
 * @param y - y coordinate of player texture
 * @param rend
 */
void playerTexture::render(int x, int y, SDL_Renderer*& rend){
	
	SDL_Rect toRender = {x, y, txtWidth, txtHeight};
	SDL_RenderCopyEx(rend, gameTexture, nullptr, &toRender, 0, nullptr, SDL_FLIP_NONE);
}

/**
 *
 * @return txtWidth - width of player texture
 */
int playerTexture::getTxtWidth(){
	return txtWidth;
}

/**
 *
 * @return  txtHeight - height of player texture
 */
int playerTexture::getTxtHeight(){
	return txtHeight;
}
