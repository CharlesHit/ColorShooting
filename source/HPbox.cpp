#include "health.h"

/**
 * initializes object of class Health Texture
 */
HPBox::HPBox(){
	hpTexture = nullptr;
}

/**
 * destructor for HealthTexture
 */
HPBox::~HPBox(){
	free();
}

/**
 * frees memory
 */
void HPBox::free(){
	if(hpTexture != nullptr){
		SDL_DestroyTexture(hpTexture);
		hpTexture = nullptr;
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
int HPBox::loadImage( const std::string image, SDL_Renderer*& rend, int HP, int scale){
	
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

	scalar = scale;
	hpHeight = (*toLoad).h;
	hpWidth = HP * scalar;

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
void HPBox::render( int x, int y, SDL_Renderer*& rend){
	SDL_Rect toRender = {x, y, hpWidth, hpHeight};
	SDL_RenderCopyEx(rend, hpTexture, nullptr, &toRender, 0, nullptr, SDL_FLIP_NONE);
}

/**
 *
 * @return hpWidth
 */
int HPBox::getHpWidth(){
	return hpWidth;
}

/**
 *
 * @return hpHeight
 */
int HPBox::getHpHeight(){
	return hpHeight;
}

/**
 *
 * @param width
 */
void HPBox::addWidth( int width) {
	hpWidth += width * scalar;
}

/**
 * sets RGB values
 * @param r - red
 * @param g - green
 * @param b - blue
 */
void HPBox::setColour( int r, int g, int b) {
	SDL_SetTextureColorMod(hpTexture, r, g, b);
}
