//
// Created by Mu He on 2019-11-04.
//

#include "bomb.h"

//
//  author: Mu He
//  @param player launched the bomb
//  @param pTxt just texture of it
//
bomb::bomb ( player * p, playerTexture * pTxt )
{
	pTexture = pTxt;
	width = ( *pTexture ).getTxtWidth();
	height = ( *pTexture ).getTxtWidth();
	xPosition = p->getXCoord();
	yPosition = p->getYCoord();
	xMovement = 0;
	yMovement = 0;
}


void bomb::handleInput ( SDL_Event &event, int direction )
{
	if(event.type == SDL_KEYUP && event.key.repeat == 0){

		if((event.key.keysym.sym == SDLK_q || event.key.keysym.sym == SDLK_u) && direction == 0){
			yMovement = yMovement - velocity; //Move up
		}

		else if((event.key.keysym.sym == SDLK_q || event.key.keysym.sym == SDLK_u) && direction == 1){
			yMovement = yMovement + velocity; //Move down
		}

		else if((event.key.keysym.sym == SDLK_q || event.key.keysym.sym == SDLK_u) && direction == 2){
			xMovement = xMovement - velocity; //Move left
		}

		else if((event.key.keysym.sym == SDLK_q || event.key.keysym.sym == SDLK_u) && direction == 3){
			xMovement = xMovement + velocity; //Move right
		}
	}

}

void bomb::launch ( int xPos, int yPos )
{

}

void bomb::render ( SDL_Renderer *&rend )
{

}

int bomb::getX ()
{
	return 0;
}

int bomb::getY ()
{
	return 0;
}

int bomb::getHeight ()
{
	return 0;
}

int bomb::getWidth ()
{
	return 0;
}
