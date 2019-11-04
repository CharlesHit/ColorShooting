//
// Created by Mu He on 2019-11-04.
//

#include "bomb.h"
#include "game.h"
#include <iostream>

//
//  author: Mu He
//  @param player launched the bomb
//  @param pTxt just texture of it
//
bomb::bomb ( player * p, playerTexture * pTxt, playerTexture * fTxt )
{
	pTexture = pTxt;
	flameTexture = fTxt;
	width = ( *pTexture ).getTxtWidth();
	height = ( *pTexture ).getTxtWidth();
	xPosition = p->getXCoord();
	yPosition = p->getYCoord();
	xMovement = 0;
	yMovement = 0;
	distance = MAX_DISTANCE;
}


void bomb::handleInput ( SDL_Event &event, int direction )
{
	if(event.type == SDL_KEYDOWN && event.key.repeat == 0 && (event.key.keysym.sym == SDLK_e || event.key.keysym.sym == SDLK_o)){
		if(direction == 0){
			yMovement = yMovement - velocity; //Move up
		}
		else if(direction == 1){
			yMovement = yMovement + velocity; //Move down
		}
		else if(direction == 2){
			xMovement = xMovement - velocity; //Move left
		}
		else if(direction == 3){
			xMovement = xMovement + velocity; //Move right
		}
	}
}

int bomb::launch ( int xPos, int yPos )
{
	yPosition = yPos + yMovement;
	xPosition = xPos + xMovement;
	distance = distance - abs(yMovement) - abs(xMovement);
	//std::cout << distance << " " << abs(yMovement) << " " << abs(xMovement) << std::endl;
	if (distance <= 0)
		return 0;
	else return 1;
}

int bomb::getRange ()
{
	return range;
}

void bomb::render ( SDL_Renderer *&rend )
{
	(*pTexture).render(xPosition, yPosition, rend);
}

void bomb::flame_render ( SDL_Renderer *&rend )
{
	(*flameTexture).render(xPosition, yPosition, rend);
}

int bomb::getX ()
{
	return xPosition;
}

int bomb::getY ()
{
	return yPosition;
}

int bomb::getHeight ()
{
	return width;
}

int bomb::getWidth ()
{
	return height;
}

int bomb::getDistance()
{
	return distance;
}
