#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>

class bullet;
class player;
class playerTexture;

class game{

	public:
		~game();
		game* getInstance();
		bool init();
		void run();
		int getScrnHeight();
		int getScrnWidth();
  		bool checkCollision(player player1, player player2);

	private:
		game();
		int SCREEN_WIDTH;
		int SCREEN_HEIGHT;
		SDL_Window* gameWindow;
		SDL_Surface* gameScreenSurface;
		SDL_Renderer* gameRenderer;
		static game* instance;
  		bool projectileCollision(player p, bullet* b);
};
