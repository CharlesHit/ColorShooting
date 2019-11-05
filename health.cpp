#include "health.h"
// This function handles health lofic
//
/**
 * initializes object of class health
 * @param startX - x coordinate of health bar
 * @param startY - y coordinate of health bar
 * @param hpTxt - starts health texture
 */
Health::Health(int startX, int startY, HealthTexture* hpTxt){
	hpTexture = hpTxt;
	width = hpTxt->getHpWidth();
	height = hpTxt->getHpHeight();
	xPosition = startX;
	yPosition = startY;
}

/**
 *
 * @return xPosition - x coordinates of health bar
 */
int Health::getXCoord()
{
    return xPosition;
}

/**
 *
 * @return yPosition - y coordinates of health bar
 */
int Health::getYCoord()
{
    return yPosition;
}

/**
 *
 * @param x - sets x coordinates
 */
void Health::setXCoord(int x)
{
  	xPosition = x;
}

/**
 *
 * @param y - sets y coordinates
 */
void Health::setYCoord(int y)
{
  	yPosition = y;
}

/**
 *
 * @return width - width of health object
 */
int Health::getWidth() {
	return width;
}

/**
 *
 * @return height - height of health object
 */
int Health::getHeight() {
	return height;
}

/**
 * Takes an SDL_renderer passed by reference and envokes playerTexture's render function
 * @param rend - renderer of health bar
 */
void Health::render(SDL_Renderer*& rend){
	(*hpTexture).render(xPosition, yPosition, rend);				
}

