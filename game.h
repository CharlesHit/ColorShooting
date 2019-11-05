#ifndef TEAM16_GAME_H
#define TEAM16_GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <iostream>
#include "bomb.h"

class bullet;
class player;
class playerTexture;

class game{

	public:
		~game();
		static game* getInstance();
		bool init();
		void run();
		int getScrnHeight();
		int getScrnWidth();
  		static bool checkCollision(player player1, player player2);

	private:
		game();
		int SCREEN_WIDTH;
		int SCREEN_HEIGHT;
		SDL_Window* gameWindow;
		SDL_Renderer* gameRenderer;
		static game* instance;
  		static bool projectileCollision(player p, bullet* b);
		static bool projectileExploded ( player p, bomb * b );
};
#endif //TEAM16_GAME_H
