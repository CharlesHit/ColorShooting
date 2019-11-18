/*
 * Date: October 25, 2019
 */
#ifndef COLORSHOOTING_PLAYERTEXTURE_H
#define COLORSHOOTING_PLAYERTEXTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class playerTexture{
		
		public:
			playerTexture();
			~playerTexture();
			void free();
			int loadImage(std::string image, SDL_Renderer*& rend);
			void render(int x, int y, SDL_Renderer*& rend);
			int getTxtWidth();
			int getTxtHeight();
			
		private:
			SDL_Texture* gameTexture;
			int txtWidth;
			int txtHeight;
};
#endif //COLORSHOOTING_PLAYERTEXTURE_H
