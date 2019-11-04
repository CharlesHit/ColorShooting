#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <iostream>

class HealthTexture {
	public:
		HealthTexture();
		~HealthTexture();
		void free();
		int loadImage(std::string image, SDL_Renderer*& rend);
		void render(int x, int y, SDL_Renderer*& rend);
		int getHpWidth();
		int getHpHeight();
		void setWidth(int width);
		void setColour(int r, int g, int b);
		
	private:
		SDL_Texture* hpTexture;
		int hpWidth;
		int hpHeight;
		
};
