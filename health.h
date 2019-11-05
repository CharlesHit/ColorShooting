#ifndef TEAM16_HEALTH_H
#define TEAM16_HEALTH_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <iostream>
#include "healthTexture.h"

class Health{
	public:
		Health(int startX, int startY, HealthTexture* hpTxt);
		void render(SDL_Renderer*& rend);
		int getXCoord();
		int getYCoord();
		int getWidth();
		int getHeight();
		void setXCoord(int x);
		void setYCoord(int y);
	private:
		int xPosition;
		int yPosition;
		int width;
		int height;
		HealthTexture* hpTexture;
};
#endif //TEAM16_HEALTH_H
