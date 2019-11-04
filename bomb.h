//
// Created by Charles on 2019-11-04.
//

#ifndef TEAM16_BOMB_H
#define TEAM16_BOMB_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>
#include "player.h"

class bomb
{

public:
	bomb ( player * p, playerTexture * pTxt );

	~bomb () = default;

	void handleInput ( SDL_Event &event, int direction );

	void launch ( int xPos, int yPos );

	void render ( SDL_Renderer *&rend );

	int getX ();

	int getY ();

	int getHeight ();

	int getWidth ();

private:
	int width;
	int height;
	int velocity = 10;
	int range = 3;
	int xPosition;
	int yPosition;
	int xMovement;
	int yMovement;
	playerTexture * pTexture;
};

#endif //TEAM16_BOMB_H
