//
// Created by Charles on 2019-11-04.
//

#ifndef COLORSHOOTING_BOMB_H
#define COLORSHOOTING_BOMB_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>
#include "player.h"

class bomb
{

public:
	bomb ( player * p, playerTexture * pTxt, playerTexture * fTxt, int explode, int max_distance );

	~bomb () = default;

	void handleInput ( SDL_Event &event, int direction );

	int launch ( int xPos, int yPos );

	void render ( SDL_Renderer *&rend );

	void flame_render ( SDL_Renderer *&rend );

	int getX ();

	int getY ();

	void setX ( int );

	void setY ( int );

	int getHeight ();

	int getWidth ();

	int getExplodingRange ();

	int getMAXDistance ();

	int getDistance ();

	int getDirection_x ();

	int getDirection_y ();

private:
	int width;
	int height;
	int range; // range of bomb
	int velocity = 10;
	int MAX_DISTANCE;
	int distance_bomb_already_go;
	int xPosition;
	int yPosition;
	int xMovement;
	int yMovement;
	playerTexture * pTexture;
	playerTexture * flameTexture;
};

#endif //COLORSHOOTING_BOMB_H
