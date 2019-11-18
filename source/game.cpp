/**
 * @brief this class initializes an instance of game class and handles the game logic
 * @authors Christopher Xu, Ivgeni Darinski, Xinwen Liang, Mu He, Nicholas Meisner
 *
 * this class initializes all the textures for the players and projectiles and
 * imports all the SDL media libraries. it also determines the coordinates of the projectiles
 * and players and renders them on the screen. it also renders health bar and hit points for
 * each player
 *
 */

#include <vector>

#include "game.h"
#include "bullet.h"
#include "bomb.h"
#include "health.h"
#include "text.h"

bool instanceFlag = false;
game * game::instance = nullptr;
Mix_Chunk * backgroundSound = nullptr;

const int HP = 400;
const int scalar = 1;

const int bomb_explode = 150; // exploding range of bomb
const int bomb_range = 400;   // throwing range of bomb

const int damage = HP / 20; // damage of bullet in HP
const int bomb_damage = 3 * damage;
const int bullet_damage_weight = 1;// damage of bullet in TEXT
const int bomb_damage_weight = bullet_damage_weight * 3;

const int MAX_STAY_time = 100; // the maximum time allowing a object remain in screen

/**
 * initializes the game screen
 */
game::game ()
{
	SCREEN_WIDTH = 1366;
	SCREEN_HEIGHT = 768;
	gameWindow = nullptr;
	gameRenderer = nullptr;
}

/**
 * destructor for game class
 */
game::~game ()
{
	//closing out and destroying everything
	instanceFlag = false;
	SDL_DestroyWindow(gameWindow);
	SDL_DestroyRenderer(gameRenderer);
	gameWindow = nullptr;
	gameRenderer = nullptr;
	SDL_Quit();
}

/**
 *
 * @return game - instance of game
 */
game * game::getInstance ()
{

	if ( !instanceFlag )
	{
		instance = new game();
		instanceFlag = true;
		return instance;
	} else
	{
		return instance;
	}
}

/**
 * this function checks collision
 * @param R - contains coordinates of player1
 * @param L - contains
 * @return true if collided false otherwise
 */
bool game::checkCollision ( player R, player L )
{
	int R_left, L_left;
	int R_top, L_top;

	R_left = R.getXCoord();
	R_top = R.getYCoord();

	L_left = L.getXCoord();
	L_top = L.getYCoord();

	return ( R_left <= L_left + L.getWidth()) &&
	       ( R_left + R.getWidth() >= L_left ) &&
	       ( R_top <= L_top + L.getHeight()) &&
	       ( R.getHeight() + R_top >= L_top );
}

/**
 *
 * initializes the gui window
 * @return true if succeeds false otherwise
 */
bool game::init ()
{

	//Initialize SDL and check that it was successful
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 )
	{
		std::cout << "SDL failed to initialize";
		return false;
	} else
	{
		//Create the window for the game to be played in
		gameWindow = SDL_CreateWindow("Color Shooting", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		//Check that the window was created properly
		if ( gameWindow == nullptr)
		{
			std::cout << "The game window could not be created";
			return false;
		} else
		{
			//render the window
			gameRenderer = SDL_CreateRenderer(gameWindow, -1, 0);

			SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

			SDL_RenderClear(gameRenderer);

			SDL_RenderPresent(gameRenderer);

			if ( Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0 )
			{
				printf("Music could not initialize! Music Error: %s\n", Mix_GetError());
			}
		}
	}

	return true;
}

/**
 *
 * handles the internal game logic and rendering of the game
 */
void game::run ()
{
HERE:
	// texture
	playerTexture Texture_R;
	playerTexture Texture_L;
	playerTexture Texture_bullet;
	playerTexture Texture_bomb; // for bomb

	// music
	SDL_AudioDeviceID deviceId;
	SDL_AudioSpec wavSpec;
	Uint32 wavLength;
	Uint8 *wavBuffer;

	// hp boxes
	HPBox Texture_red;
	HPBox HPBox_Left;
	HPBox HPBox_Right;

	// asset loading
	Texture_R.loadImage("asset/player_R.bmp", gameRenderer);
	Texture_L.loadImage("asset/player_L.bmp", gameRenderer);
	Texture_bullet.loadImage("asset/dot.bmp", gameRenderer);
	Texture_bomb.loadImage("asset/flame.bmp", gameRenderer);
	Texture_red.loadImage("asset/player_R.bmp", gameRenderer, HP, scalar);
	HPBox_Left.loadImage("asset/player_R.bmp", gameRenderer, HP, scalar);
	HPBox_Right.loadImage("asset/player_R.bmp", gameRenderer, HP, scalar);
	HPBox_Left.setColour(50, 205, 50);
	HPBox_Right.setColour(50, 205, 50);

	player R(SCREEN_WIDTH - 80, SCREEN_HEIGHT / 2, &Texture_R, HP);
	player L(80, SCREEN_HEIGHT / 2, &Texture_L, HP);

	Health HP_R_red(50, 50, &Texture_red);
	Health HP_L_red(SCREEN_WIDTH - HPBox_Left.getHpWidth() - 50, 50, &Texture_red);
	Health HP_R_green(50, 50, &HPBox_Left);
	Health HP_L_green(SCREEN_WIDTH - HPBox_Left.getHpWidth() - 50, 50, &HPBox_Right);

	// instance of BULLET
	std::vector<bullet *> bullets_R;
	std::vector<bullet *> bullets_L;
	std::vector<bool> bullets_R_boolean;
	std::vector<bool> bullets_L_boolean;
	bullet * bul_R = nullptr;   // a single bul
	bullet * bul_L = nullptr;   // a single bul

	// instance of BOMB
	std::vector<bomb *> bombs_R;
	std::vector<bomb *> bombs_L;
	std::vector<bool> bombs_R_boolean;
	std::vector<bool> bombs_L_boolean;
	bomb * bomb_R = nullptr;
	bomb * bomb_L = nullptr;

	// instance of font and initialization of score table (not a good way though)
	TTF_Init();
	SDL_Color color;
	color.r = 100;
	color.b = 0;
	color.g = 0;
	int damage_each_hit = HP / 5; // basic damage hit. normally for one bullet
	Text * Score_R[damage_each_hit + 1];
	Text * Score_L[damage_each_hit + 1];
	int HP_remaining = HP;
	int TotalDamage_Left = 0;
	int TotalDamage_Right = 0;
	for ( int i = 0; i < damage_each_hit; i++ )
	{
		auto s = std::to_string(HP_remaining);

		try {
			Score_R[i] = new Text(gameRenderer, "Roboto-Regular.ttf", 36, s, color);
			Score_L[i] = new Text(gameRenderer, "Roboto-Regular.ttf", 36, s, color);
		} catch (int e)
		{
			std::cout << "Line 228: Failed to load font!" << std::endl;
			exit(1);
		}
		HP_remaining -= damage;
	}

	// text for wining screen
	Text * L_wins;
	Text * R_wins;
	Text * restartText;

	// Music upload
	try
	{
		SDL_Init(SDL_INIT_AUDIO);

		SDL_LoadWAV("asset/6-final-screen.wav", &wavSpec, &wavBuffer, &wavLength);

		deviceId = SDL_OpenAudioDevice(nullptr, 0, &wavSpec, nullptr, 0);

		int success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
		SDL_PauseAudioDevice(deviceId, 0);
	}
	catch ( int e )
	{
		std::cout << "Huh! No music! - " << Mix_GetError() << std::endl;
		exit(1);
	}

	// Environment Setting
	bool quit = false;
	SDL_Event event;
	int currentTime_R = 0;
	int diffTime_R = 0;
	int lastTime_R = 0;
	int accumulator_R = 0;
	int currentTime_L = 0;
	int diffTime_L = 0;
	int lastTime_L = 0;
	int accumulator_L = 0;

	// this is for some object that will stay in screen for seconds
	bool stay_R = false;
	int stay_R_time = MAX_STAY_time;
	bool stay_L = false;
	int stay_L_time = MAX_STAY_time;

	// final decider
	bool R_lost = false;
	bool L_lost = false;

	//While user does not quit
	while ( !quit )
	{

		int R_x = R.getXCoord();
		int R_y = R.getYCoord();
		int L_x = L.getXCoord();
		int L_y = L.getYCoord();

		currentTime_R = SDL_GetTicks();
		diffTime_R = currentTime_R - lastTime_R;
		accumulator_R += diffTime_R;
		lastTime_R = currentTime_R;

		currentTime_L = SDL_GetTicks();
		diffTime_L = currentTime_L - lastTime_L;
		accumulator_L += diffTime_L;
		lastTime_L = currentTime_L;

		//Handle events on queue
		while ( SDL_PollEvent(&event) != 0 )
		{

			//Check for user quit
			if ( event.type == SDL_QUIT )
				quit = true;

			//If the up, down, left, or right arrows keys are used then this is a move event to be handled by R
			if ( event.key.keysym.sym == SDLK_i || event.key.keysym.sym == SDLK_k || event.key.keysym.sym == SDLK_j ||
			     event.key.keysym.sym == SDLK_l )
				R.handleMoveEvent(event);

				//If the w, a, s, or d keys are used then this is a move event to be handled by L
			else if ( event.key.keysym.sym == SDLK_w || event.key.keysym.sym == SDLK_s ||
			          event.key.keysym.sym == SDLK_a || event.key.keysym.sym == SDLK_d )
				L.handleMoveEvent(event);

				// bullet, L, pressing q
			else if ( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q && accumulator_R > 500 )
			{
				accumulator_R -= 500;
				bul_L = new bullet(L.getXCoord(), L.getYCoord(), &Texture_bullet);
				( *bul_L ).handleInput(event, L.getDirection());
				( *bul_L ).shoot(L_x, L_y);
				bullets_L.push_back(bul_L);
				bullets_L_boolean.push_back(true);
			}

				// bomb, L, pressing e
			else if ( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_e && accumulator_R > 500 )
			{
				accumulator_R -= 500;
				bomb_L = new bomb(&L, &Texture_bullet, &Texture_bomb, bomb_explode, bomb_range);
				( *bomb_L ).handleInput(event, L.getDirection());
				( *bomb_L ).launch(L_x, L_y);
				bombs_L.push_back(bomb_L);
				bombs_L_boolean.push_back(true);
			}

				// bullet, R, pressing u
			else if ( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_u && accumulator_L > 500 )
			{
				accumulator_L -= 500;
				bul_R = new bullet(R.getXCoord(), R.getYCoord(), &Texture_bullet);
				( *bul_R ).handleInput(event, R.getDirection());
				( *bul_R ).shoot(R_x, R_y);
				bullets_R.push_back(bul_R);
				bullets_R_boolean.push_back(true);
			}

				// bomb, R, pressing o
			else if ( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_o && accumulator_R > 500 )
			{
				accumulator_R -= 500;
				bomb_R = new bomb(&R, &Texture_bullet, &Texture_bomb, bomb_explode, bomb_range);
				( *bomb_R ).handleInput(event, R.getDirection());
				( *bomb_R ).launch(R_x, R_y);
				bombs_R.push_back(bomb_R);
				bombs_R_boolean.push_back(true);
			}
		}

		R.move();
		L.move();

		if ( checkCollision(R, L))
		{
			R.setXCoord(R.getXCoord() - R.getXMovement());
			R.setYCoord(R.getYCoord() - R.getYMovement());
			L.setXCoord(L.getXCoord() - L.getXMovement());
			L.setYCoord(L.getYCoord() - L.getYMovement());
		}

		//Clear the screen to remove the players' previous positions
		SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gameRenderer);

		//Render the players in their new positions
		R.render(gameRenderer);
		L.render(gameRenderer);

		HP_R_red.render(gameRenderer);
		HP_L_red.render(gameRenderer);
		HP_R_green.render(gameRenderer);
		HP_L_green.render(gameRenderer);

		Score_R[TotalDamage_Left]->display(HP_R_red.getXCoord(), HP_R_red.getYCoord() + HP_R_red.getHeight(),
		                                   gameRenderer);
		Score_L[TotalDamage_Right]->display(HP_L_red.getXCoord(), HP_L_red.getYCoord() + HP_R_red.getHeight(),
		                                    gameRenderer);

		// score check and end game decider
		if ( L.getHealth() <= 0 )
		{
			L_lost = true;
			TotalDamage_Left = 0;
			try
			{
				L_wins = new Text(gameRenderer, "asset/Roboto-Regular.ttf", 80, "Right guy wins!", color);
				restartText = new Text(gameRenderer, "asset/Roboto-Regular.ttf", 80, "Play again ? (y/n)", color);
			} catch ( int e )
			{
				std::cout << "Victory Font for Right guy error" << std::endl;
				exit(1);
			}
			L_wins->display(400, 300, gameRenderer);
			restartText->display(400, 500, gameRenderer);
			Mix_Volume(-1, MIX_MAX_VOLUME / 6);
			Mix_PlayChannel(-1, backgroundSound, 0);
		}

		if ( R.getHealth() <= 0 )
		{
			R_lost = true;
			TotalDamage_Right = 0;
			try
			{
				R_wins = new Text(gameRenderer, "asset/Roboto-Regular.ttf", 80, "Left guys wins!", color);
				restartText = new Text(gameRenderer, "asset/Roboto-Regular.ttf", 80, "Play again(y/n)", color);
			}
			catch ( int e )
			{
				std::cout << "Victory Font for Left guy error" << std::endl;
				exit(1);
			}
			R_wins->display(400, 300, gameRenderer);
			restartText->display(400, 500, gameRenderer);
			Mix_Volume(-1, MIX_MAX_VOLUME / 6);
			Mix_PlayChannel(-1, backgroundSound, 0);
		}

		if ( L_lost )
		{
			L.setXCoord(15000);
			L.setYCoord(15000);
		}

		if ( R_lost )
		{
			R.setXCoord(15000);
			R.setYCoord(15000);
		}

		//Render the players in their new positions
		R.render(gameRenderer);
		L.render(gameRenderer);


		if ( R.getHealth() <= 0 || L.getHealth() <= 0 )
		{

			SDL_RenderPresent(gameRenderer);
			while ( true )
			{
				while ( SDL_PollEvent(&event))
				{
					if ( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_n )
					{
						exit(0);
					}
					if ( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_y )
					{

						Mix_HaltChannel(-1);

						goto HERE;
					}
				}
			}
		}

		// bullet, R
		for ( int i = 0; i < ( int ) bullets_R.size(); i++ )
		{
			if ( bullets_R_boolean[i] )
			{
				( *bullets_R[i] ).shoot(( *bullets_R[i] ).getX(), ( *bullets_R[i] ).getY());

				if (( *bullets_R[i] ).getY() < 0 ||
				    ( *bullets_R[i] ).getY() + ( *bullets_R[i] ).getHeight() > SCREEN_HEIGHT ||
				    projectileCollision(L, bullets_R[i]))
				{
					bullets_R_boolean[i] = false;

					if ( projectileCollision(L, bullets_R[i]))
					{
						L.setHealth(L.getHealth() - damage);
						// std::cout << "Player 2 took 10 damage: " << L.getHealth() << std::endl;
						// for hp text
						TotalDamage_Left += bullet_damage_weight;
						// for hp bar
						HPBox_Left.addWidth(-damage);
					}
				} else if (( *bullets_R[i] ).getX() < 0 ||
				           ( *bullets_R[i] ).getX() + ( *bullets_R[i] ).getWidth() > SCREEN_WIDTH ||
				           projectileCollision(L, bullets_R[i]))
				{
					bullets_R_boolean[i] = false;

					if ( projectileCollision(L, bullets_R[i]))
					{
						L.setHealth(L.getHealth() - damage);
						// std::cout << "Player 2 took 10 damage: " << L.getHealth() << std::endl;
						// for hp text
						TotalDamage_Left += bullet_damage_weight;
						// for hp bar
						HPBox_Left.addWidth(-damage);

					}
				}

				if ( !bullets_R_boolean[i] )
				{
					delete bullets_R[i];
					bullets_R[i] = nullptr;
				} else
				{
					( *bullets_R[i] ).render(gameRenderer);
				}
			}
		}

		// bullet, L
		for ( int i = 0; i < ( int ) bullets_L.size(); i++ )
		{
			if ( bullets_L_boolean[i] )
			{
				( *bullets_L[i] ).shoot(( *bullets_L[i] ).getX(), ( *bullets_L[i] ).getY());

				if (( *bullets_L[i] ).getY() < 0 ||
				    ( *bullets_L[i] ).getY() + ( *bullets_L[i] ).getHeight() > SCREEN_HEIGHT ||
				    projectileCollision(R, bullets_L[i]))
				{
					bullets_L_boolean[i] = false;

					if ( projectileCollision(R, bullets_L[i]))
					{
						R.setHealth(R.getHealth() - damage);
						// for HP text
						TotalDamage_Right += bullet_damage_weight;
						// for HP bar
						HPBox_Right.addWidth(-damage);
					}

				} else if (( *bullets_L[i] ).getX() < 0 ||
				           ( *bullets_L[i] ).getX() + ( *bullets_L[i] ).getWidth() > SCREEN_WIDTH ||
				           projectileCollision(R, bullets_L[i]))
				{
					bullets_L_boolean[i] = false;

					if ( projectileCollision(R, bullets_L[i]))
					{
						R.setHealth(R.getHealth() - damage);
						// for HP text
						TotalDamage_Right += bullet_damage_weight;
						// for HP bar
						HPBox_Right.addWidth(-damage);
					}
				}

				if ( !bullets_L_boolean[i] )
				{
					delete bullets_L[i];
					bullets_L[i] = nullptr;
				} else
				{
					( *bullets_L[i] ).render(gameRenderer);
				}
			}
		}

		// bomb L
		int bombL_X_modified, bombL_Y_modified;
		int bombL_X, bombL_Y;
		if ( stay_L && stay_L_time > 0 )
		{
			( Texture_bomb ).render(bombL_X_modified, bombL_Y_modified, gameRenderer);
			stay_L_time--;
		}
		if ( stay_L_time == 0 )
		{
			stay_L = false;
			stay_L_time = MAX_STAY_time;
		}
		for ( int i = 0; i < ( int ) bombs_L_boolean.size(); i++ )
		{
			if ( bombs_L_boolean[i] )
			{
				if ((( *bombs_L[i] ).getY() < 0 ||
				     ( *bombs_L[i] ).getY() + ( *bombs_L[i] ).getHeight() > SCREEN_HEIGHT )
				    ||
				    (( *bombs_L[i] ).getX() < 0 || ( *bombs_L[i] ).getX() + ( *bombs_L[i] ).getWidth() > SCREEN_WIDTH ))
				{
					bombs_L_boolean[i] = false;
					if ( bombExploded(R, L, bombs_L[i]))
					{
						R.setHealth(R.getHealth() - bomb_damage);
						// for HP text
						TotalDamage_Right += bomb_damage_weight;
						// for HP bar
						HPBox_Right.addWidth(-bomb_damage);
					}
				}

				// in bomb, largest flying distance_bomb_already_go is MAX distance_bomb_already_go
				if (( *bombs_L[i] ).launch(( *bombs_L[i] ).getX(), ( *bombs_L[i] ).getY()) == 0 )
					bombs_L_boolean[i] = false;

				if ( !bombs_L_boolean[i] )
				{
					// Notice here: x_, y_ is the centre of the bomb, it is equals to:
					// |       o <- bomb
					// |
					// player________
					bomb * b = new bomb(&L, &Texture_bullet, &Texture_bomb, bomb_explode, bomb_range);
					switch ( L.getDirection())
					{
						case 0: // 0 is up. See document in player.cpp
							bombL_X = L_x;
							bombL_X_modified = bombL_X - bomb_explode;
							bombL_Y = L_y - bomb_range;
							bombL_Y_modified = bombL_Y - bomb_explode;
							break;
						case 1: // 1 is down
							bombL_X = L_x;
							bombL_X_modified = bombL_X - bomb_explode;
							bombL_Y = L_y + bomb_range;
							bombL_Y_modified = bombL_Y - bomb_explode;
							break;
						case 2: // 2 is left
							bombL_X = L_x - bomb_range;
							bombL_X_modified = bombL_X - bomb_explode;
							bombL_Y = L_y;
							bombL_Y_modified = bombL_Y - bomb_explode;
							break;
						case 3: // 3 is right
							bombL_X = L_x + bomb_range;
							bombL_X_modified = bombL_X - bomb_explode;
							bombL_Y = L_y;
							bombL_Y_modified = bombL_Y - bomb_explode;
							break;
					}
					stay_L = true;
					delete bombs_L[i];

					b->setX(bombL_X);
					b->setY(bombL_Y);

					if ( bombExploded(R, L, b))
					{
						R.setHealth(R.getHealth() - bomb_damage);
						// for HP text
						TotalDamage_Right += bomb_damage_weight;
						// for HP bar
						HPBox_Right.addWidth(-bomb_damage);
					}
				}
			}
		}

		// bomb R
		int bombR_X_modified, bombR_Y_modified;
		int bombR_X, bombR_Y;
		if ( stay_R && stay_R_time > 0 )
		{
			( Texture_bomb ).render(bombR_X_modified, bombR_Y_modified, gameRenderer);
			stay_R_time--;
		}
		if ( stay_R_time == 0 )
		{
			stay_R = false;
			stay_R_time = MAX_STAY_time;
		}
		for ( int i = 0; i < ( int ) bombs_R_boolean.size(); i++ )
		{
			if ( bombs_R_boolean[i] )
			{
				if ((( *bombs_R[i] ).getY() < 0 ||
				     ( *bombs_R[i] ).getY() + ( *bombs_R[i] ).getHeight() > SCREEN_HEIGHT )
				    ||
				    (( *bombs_R[i] ).getX() < 0 || ( *bombs_R[i] ).getX() + ( *bombs_R[i] ).getWidth() > SCREEN_WIDTH ))
				{
					bombs_R_boolean[i] = false;
					if ( bombExploded(L, R, bombs_R[i]))
					{
						L.setHealth(L.getHealth() - bomb_damage);
						// for HP text
						TotalDamage_Left += bomb_damage_weight;
						// for HP bar
						HPBox_Left.addWidth(-bomb_damage);
					}
				}

				// in bomb, largest flying distance_bomb_already_go is MAX distance_bomb_already_go
				if (( *bombs_R[i] ).launch(( *bombs_R[i] ).getX(), ( *bombs_R[i] ).getY()) == 0 )
					bombs_R_boolean[i] = false;

				if ( !bombs_R_boolean[i] )
				{
					// Notice here: x_, y_ is the centre of the bomb, it is equals to:
					// |       o <- bomb
					// |
					// player________
					bomb * b = new bomb(&R, &Texture_bullet, &Texture_bomb, bomb_explode, bomb_range);
					switch ( R.getDirection())
					{
						case 0: // 0 is up. See document in player.cpp
							bombR_X = R_x;
							bombR_X_modified = bombR_X - bomb_explode;
							bombR_Y = R_y - bomb_range;
							bombR_Y_modified = bombR_Y - bomb_explode;
							break;
						case 1: // 1 is down
							bombR_X = R_x;
							bombR_X_modified = bombR_X - bomb_explode;
							bombR_Y = R_y + bomb_range;
							bombR_Y_modified = bombR_Y - bomb_explode;
							break;
						case 2: // 2 is left
							bombR_X = R_x - bomb_range;
							bombR_X_modified = bombR_X - bomb_explode;
							bombR_Y = R_y;
							bombR_Y_modified = bombR_Y - bomb_explode;
							break;
						case 3: // 3 is right
							bombR_X = R_x + bomb_range;
							bombR_X_modified = bombR_X - bomb_explode;
							bombR_Y = R_y;
							bombR_Y_modified = bombR_Y - bomb_explode;
							break;
					}
					stay_R = true;
					delete bombs_R[i];

					b->setX(bombR_X);
					b->setY(bombR_Y);

					if ( bombExploded(L, R, b))
					{
						L.setHealth(L.getHealth() - bomb_damage);
						// for HP text
						TotalDamage_Left += bomb_damage_weight;
						// for HP bar
						HPBox_Left.addWidth(-bomb_damage);
					}

				}
			}
		}

		//Update the game screen
		SDL_RenderPresent(gameRenderer);
	}

	SDL_CloseAudioDevice(deviceId);
	SDL_FreeWAV(wavBuffer);

	std::cout << "exit" << std::endl;

	Texture_R.free();
	Texture_L.free();
	Texture_bullet.free();
	Texture_red.free();
	HPBox_Left.free();
	HPBox_Right.free();
}

int game::getScrnHeight ()
{
	return SCREEN_HEIGHT;
}

int game::getScrnWidth ()
{
	return SCREEN_WIDTH;
}

// Checks if there is a collision between a player and a bullet
bool game::projectileCollision ( player p, bullet * b )
{
	return p.getXCoord() <= b->getX() + b->getWidth() && // Checks if player is to the left of bullet
	       p.getXCoord() + p.getWidth() >= b->getX() && // Checks if player is to the right of bullet
	       p.getYCoord() <= b->getY() + b->getHeight() &&
	       p.getYCoord() + p.getHeight() >= b->getY();

}

// Checks if there is a collision between a player and a bullet
bool game::bombExploded ( player p, player p2, bomb * b )
{
//	std::cout << "bomb:" << b->getX() << " " << b->getY() << std::endl;
//	std::cout << "player L:" << p.getXCoord() << " " << p.getYCoord() << std::endl;
//	std::cout << "player R:" << p2.getXCoord() << " " << p2.getYCoord() << std::endl;
//
//	std::cout << (( b->getX() - p.getXCoord()) * ( b->getX() - p.getXCoord()) +
//	              ( b->getY() - p.getYCoord()) * ( b->getY() - p.getYCoord())) << std::endl;
//	std::cout << b->getExplodingRange() * b->getExplodingRange() << std::endl;
	return (( b->getX() - p.getXCoord()) * ( b->getX() - p.getXCoord()) +
	        ( b->getY() - p.getYCoord()) * ( b->getY() - p.getYCoord()))
	       <= bomb_explode * bomb_explode;
}

/**
 * starts execution of game
 * @param argc
 * @param args
 * @return 0 if succeeds
 */
int main ( int argc, char * args[] )
{

	game * currentGame = nullptr;
	currentGame = ( *currentGame ).getInstance();

	if ( !( *currentGame ).init())
	{
		std::cout << "Failed to initialize, program is exiting.\n";
	} else
	{
		( *currentGame ).run();
	}

	delete currentGame;
	return 0;
}

