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
bomb::bomb ( player * p, playerTexture * pTxt, playerTexture * fTxt, int explode, int max_distance )
{
	pTexture = pTxt;
	flameTexture = fTxt;
	width = ( *pTexture ).getTxtWidth();
	height = ( *pTexture ).getTxtWidth();
	xPosition = p->getXCoord();
	yPosition = p->getYCoord();
	xMovement = 0;
	yMovement = 0;
	distance_bomb_already_go = MAX_DISTANCE;
	range = explode;
	MAX_DISTANCE = max_distance;
}


void bomb::handleInput ( SDL_Event &event, int direction )
{
	if ( event.type == SDL_KEYDOWN && event.key.repeat == 0 &&
	     ( event.key.keysym.sym == SDLK_e || event.key.keysym.sym == SDLK_o ))
	{
		if ( direction == 0 )
		{
			yMovement = yMovement - velocity; //Move up
		} else if ( direction == 1 )
		{
			yMovement = yMovement + velocity; //Move down
		} else if ( direction == 2 )
		{
			xMovement = xMovement - velocity; //Move left
		} else if ( direction == 3 )
		{
			xMovement = xMovement + velocity; //Move right
		}
	}
}

int bomb::launch ( int xPos, int yPos )
{
	yPosition = yPos + yMovement;
	xPosition = xPos + xMovement;
	distance_bomb_already_go = distance_bomb_already_go - abs(yMovement) - abs(xMovement);
	if ( distance_bomb_already_go < 0 )
		return 0;
	return 1;
}

int bomb::getExplodingRange ()
{
	return range;
}

void bomb::render ( SDL_Renderer *&rend )
{
	( *pTexture ).render(xPosition, yPosition, rend);
}

void bomb::flame_render ( SDL_Renderer *&rend )
{
	( *flameTexture ).render(xPosition, yPosition, rend);
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

int bomb::getMAXDistance ()
{
	return MAX_DISTANCE;
}

int bomb::getDistance ()
{
	return distance_bomb_already_go;
}

int bomb::getDirection_x ()
{
	return xMovement == 0 ? 0 : ( int ) xMovement / abs(xMovement);
}

int bomb::getDirection_y ()
{
	return yMovement == 0 ? 0 : ( int ) yMovement / abs(yMovement);
}

void bomb::setX ( int new_x )
{
	xPosition = new_x;
}

void bomb::setY ( int new_y )
{
	yPosition = new_y;
}
