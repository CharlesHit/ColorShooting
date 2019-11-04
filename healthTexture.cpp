#include "health.h"

/**
 * initializes object of class Health Texture
 */
HealthTexture::HealthTexture(){
	hpTexture = NULL;
	hpWidth = 0;
	hpHeight = 0;
}

/**
 * destructor for HealthTexture
 */
HealthTexture::~HealthTexture(){
	free();
}

/**
 * frees memory
 */
void HealthTexture::free(){
	if(hpTexture != NULL){
		SDL_DestroyTexture(hpTexture);
		hpTexture = NULL;
		hpWidth = 0;
		hpHeight = 0;
	}
}

/**
 * loads image
 * @param image - image to be loaded
 * @param rend - renderer for health texture
 * @return 0 if success -1 or -2 on error
 */
int HealthTexture::loadImage(std::string image, SDL_Renderer*& rend){
	
	free();
	SDL_Texture* newTexture;
	SDL_Surface* toLoad = IMG_Load(image.c_str());
	if(toLoad == NULL){
		return(-1);
	}
	
	SDL_SetColorKey(toLoad, SDL_TRUE, SDL_MapRGB((*toLoad).format, 0, 0xFF, 0xFF));
	
	newTexture = SDL_CreateTextureFromSurface(rend, toLoad);
	if(newTexture == NULL){
		return(-2);
	}
	
	hpWidth = 250;
	hpHeight = (*toLoad).h;
	
	SDL_FreeSurface(toLoad);
	
	hpTexture = newTexture;
	return 0;
	
}

/**
 *
 * @param x - x coordinate of HealthTexture
 * @param y - y coordinate of HealthTexture
 * @param rend - returns renderer of Health Texture
 */
void HealthTexture::render(int x, int y, SDL_Renderer*& rend){
	SDL_Rect toRender = {x, y, hpWidth, hpHeight};
	SDL_RenderCopyEx(rend, hpTexture, NULL, &toRender, 0, NULL, SDL_FLIP_NONE);
}

/**
 *
 * @return hpWidth
 */
int HealthTexture::getHpWidth(){
	return hpWidth;
}

/**
 *
 * @return hpHeight
 */
int HealthTexture::getHpHeight(){
	return hpHeight;
}

/**
 *
 * @param width
 */
void HealthTexture::setWidth(int width) {
	hpWidth = width;
}

/**
 * sets RGB values
 * @param r - red
 * @param g - green
 * @param b - blue
 */
void HealthTexture::setColour(int r, int g, int b) {
	SDL_SetTextureColorMod(hpTexture, r, g, b);
}
