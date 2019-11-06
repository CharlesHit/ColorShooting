#ifndef TEAM16_HPBOX_H
#define TEAM16_HPBOX_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <iostream>

class HPBox {
	public:
		HPBox();
		~HPBox();
		void free();
		int loadImage(std::string image, SDL_Renderer*& rend, int HP, int scale);
		void render(int x, int y, SDL_Renderer*& rend);
		int getHpWidth();
		int getHpHeight();
		void addWidth( int width);
		void setColour(int r, int g, int b);
		
	private:
		SDL_Texture* hpTexture;
		int hpWidth;
		int hpHeight;
		int scalar; // the rate to scalar
		
};

#endif //TEAM16_HPBOX_H
