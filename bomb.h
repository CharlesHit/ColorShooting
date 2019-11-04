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
	bomb ( player * p, playerTexture * pTxt, playerTexture * fTxt );

	~bomb () = default;

	void handleInput ( SDL_Event &event, int direction );

	int launch ( int xPos, int yPos );

	void render ( SDL_Renderer *&rend );

	void flame_render ( SDL_Renderer *&rend );

	int getX ();

	int getY ();

	int getHeight ();

	int getWidth ();

	int getRange ();

	int getDistance();

private:
	int width;
	int height;
	const int range = 300; // range of bomb
	int velocity = 10;
	const int MAX_DISTANCE = 500;
	int distance;
	int xPosition;
	int yPosition;
	int xMovement;
	int yMovement;
	playerTexture * pTexture;
	playerTexture * flameTexture;
};

#endif //TEAM16_BOMB_H
