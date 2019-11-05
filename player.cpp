/* Code by: Group 16
 * Date: October 24, 2019
 */

#include "player.h"
#include "game.h"

const int UP = 0;
const int DOWN = 1;
const int LEFT = 2;
const int RIGHT = 3;

/**
 * constructor for player class
 * @param startX - x coordinates of player
 * @param startY - y coordinates of player
 * @param pTxt - player texture of player
 */
player::player(int startX, int startY, playerTexture* pTxt, int HP){
	pTexture = pTxt;
	width = (*pTexture).getTxtWidth();
	height = (*pTexture).getTxtWidth();
	xPosition = startX;
	yPosition = startY;
	xMovement = 0;
	yMovement = 0;
	direction = 0;
	cooldown = 0;
	health = HP;
}

/**
 * determines velcoity of player
 * @param event - keyboard input
 */
void player::handleMoveEvent(SDL_Event& event){
	
	//Checks if the event is a button being pushed down and prevents key repeats, have to move/render new position before handling another event
	if(event.type == SDL_KEYDOWN && event.key.repeat == 0){
		
		if(event.key.keysym.sym == SDLK_i || event.key.keysym.sym == SDLK_w){
			yMovement = yMovement - velocity;					//Adjust velocity to move upwards
            direction = UP;
		}
		
		else if(event.key.keysym.sym == SDLK_k || event.key.keysym.sym == SDLK_s){
			yMovement = yMovement + velocity;					//Adjust velocity to move downwards
            direction = DOWN;

		}
		
		else if(event.key.keysym.sym == SDLK_j || event.key.keysym.sym == SDLK_a){
			xMovement = xMovement - velocity;					//Adjust velocity to move left
            direction = LEFT;
		}
		
		else if(event.key.keysym.sym == SDLK_l || event.key.keysym.sym == SDLK_d){
			xMovement = xMovement + velocity;					//Adjust velocity to move right
            direction = RIGHT;
		}
	}
	
	//Checks if the event is a button being released and prevents key repeats
	else if(event.type == SDL_KEYUP && event.key.repeat == 0){
		
		if(event.key.keysym.sym == SDLK_i || event.key.keysym.sym == SDLK_w){
			yMovement = yMovement + velocity;					//Stop moving up
		}
		
		else if(event.key.keysym.sym == SDLK_k || event.key.keysym.sym == SDLK_s){
			yMovement = yMovement - velocity;					//Stop moving down
		}
		
		else if(event.key.keysym.sym == SDLK_j || event.key.keysym.sym == SDLK_a){
			xMovement = xMovement + velocity;					//Stop moving left
		}
		
		else if(event.key.keysym.sym == SDLK_l || event.key.keysym.sym == SDLK_d){
			xMovement = xMovement - velocity;					//Stop moving right
		}
	}
}

/**
 *
 * @return xPosition - x coordinates of player
 */
int player::getXCoord()
{
    return xPosition;
}

/**
 *
 * @return yPosition - y coordinates of player
 */
int player::getYCoord()
{
    return yPosition;
}

/**
 * changes x coordinate
 * @param x - x coordinate
 */
void player::setXCoord(int x)
{
  	xPosition = x;
}

/**
 * changes y coordinate
 * @param y - y coordinate
 */
void player::setYCoord(int y)
{
  	yPosition = y;
}

/**
 *
 * @return yMovement - velocity of player in y axis
 */
int player::getYMovement(){
    return yMovement;
}

/**
 *
 * @return xMovement - velocity of player in x axis
 */
int player::getXMovement(){
    return xMovement;
}

/**
 *
 * @return direction - direction of player
 */
int player::getDirection() {
    return direction;
}

/**
 *
 * @return width - width of player
 */
int player::getWidth() {
	return width;
}

/**
 *
 * @return height - height of player
 */
int player::getHeight() {
	return height;
}

/**
 *
 * @return health - health of player
 */
int player::getHealth() {
	return health;
}

/**
 * changes health of player
 * @param h - health of player
 */
void player::setHealth(int h) {
	health = h;
}

/**
 * changes coordinates of player
 */
void player::move(){
	
	game* currentGame = (*currentGame).getInstance();
	
	yPosition = yPosition + yMovement;							//Move up/down
	if (yPosition < 0 || yPosition + height > (*currentGame).getScrnHeight()){ 
		yPosition = yPosition - yMovement;						//readjust positioning if it will be outside the screen
		
	}
	
	xPosition = xPosition + xMovement;							//Move left/right
	if (xPosition < 0 || xPosition + width > (*currentGame).getScrnWidth()){
		xPosition = xPosition - xMovement;						//readjust positioning if it will be outside the screen
	}
}

/**
 *
 * @param rend - renderer
 */
//Take an SDL_renderer passed by reference and envokes playerTexture's render function
void player::render(SDL_Renderer*& rend){
	(*pTexture).render(xPosition, yPosition, rend);				
}

