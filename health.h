#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "healthTexture.h"

class Health{
	public:
		Health(int startX, int startY, HealthTexture* hpTxt);
		void handleMoveEvent(SDL_Event& event);
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
