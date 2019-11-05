//
// Created by Christopher Xu on 2019-10-26.
// Reviewed by Charles on 2019-11-4.
//

#include "game.h"
#include "bullet.h"
#include "bomb.h"
#include "health.h"
#include <vector>

bool instanceFlag = false;
game* game::instance = nullptr;

const int damage = 10;
const int bomb_damage = 30;

const int STAY = 100;

/**
 * initializes the game screen
 */
game::game(){
    SCREEN_WIDTH = 1366;
    SCREEN_HEIGHT = 768;
    gameWindow = nullptr;
    gameRenderer = nullptr;
}

/**
 * destructor for game class
 */
game::~game(){
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
game* game::getInstance(){

    if(!instanceFlag){
        instance = new game();
        instanceFlag = true;
        return instance;
    }

    else{
        return instance;
    }
}

/**
 * this function checks collision
 * @param player1 - contains coordinates of player1
 * @param player2 - contains
 * @return true if collided false otherwise
 */
bool game::checkCollision(player player1, player player2)
{
    int leftp1, leftp2;
    int rightp1, rightp2;
    int topp1, topp2;
    int bottomp1, bottomp2;
    
    leftp1 = player1.getXCoord();
    rightp1 = player1.getXCoord() + player1.getWidth();
    topp1 = player1.getYCoord();
    bottomp1 = player1.getYCoord() + player1.getHeight();
    
    leftp2 = player2.getXCoord();
    rightp2 = player2.getXCoord() + player2.getWidth();
    topp2 = player2.getYCoord();
    bottomp2 = player2.getYCoord() + player2.getHeight();

	return ( leftp1 <= leftp2 + player2.getWidth()) &&
	       ( leftp1 + player1.getWidth() >= leftp2 ) &&
	       ( topp1 <= topp2 + player2.getHeight()) &&
	       ( player1.getHeight() + topp1 >= topp2 );

}

/**
 *
 * initializes the gui window
 * @return true if succeeds false otherwise
 */
bool game::init(){

    //Initialize SDL and check that it was successful
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
        std::cout << "SDL failed to initialize";
        return false;
    }

    else{
        //Create the window for the game to be played in
        gameWindow = SDL_CreateWindow( "Two Player Shooter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );

        //Check that the window was created properly
        if( gameWindow == nullptr ){
            std::cout << "The game window could not be created";
            return false;
        }

        else{
            //render the window
            gameRenderer = SDL_CreateRenderer(gameWindow, -1, 0);

            SDL_SetRenderDrawColor( gameRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

            SDL_RenderClear(gameRenderer);

            SDL_RenderPresent(gameRenderer);
        }
    }

    return true;
}

/**
 *
 * handles the internal game logic and rendering of the game
 */
void game::run(){

    playerTexture oneTexture;
    playerTexture twoTexture;
    playerTexture projectile;
	playerTexture flameTexture; // for bomb
    
    HealthTexture hp_red_texture;
    HealthTexture hp1_texture;
    HealthTexture hp2_texture;
    
    oneTexture.loadImage("player1.bmp", gameRenderer);
    twoTexture.loadImage("player2.bmp", gameRenderer);
    projectile.loadImage("dot.bmp", gameRenderer);
	flameTexture.loadImage("flame.bmp", gameRenderer);
    hp_red_texture.loadImage("player1.bmp", gameRenderer);
    hp1_texture.loadImage("player1.bmp", gameRenderer);
    hp2_texture.loadImage("player1.bmp", gameRenderer);
    hp1_texture.setColour(50, 205, 50);
    hp2_texture.setColour(50, 205, 50);

    player square1(1286,384, &oneTexture);
    player square2(50,384, &twoTexture);
    
    Health hp1_red(50, 50, &hp_red_texture);
    Health hp2_red(SCREEN_WIDTH - 300, 50, &hp_red_texture);
    Health hp1_green(50, 50, &hp1_texture);
    Health hp2_green(SCREEN_WIDTH - 300, 50, &hp2_texture);

	// instance of BULLET
    std::vector<bullet*> p1bullets;
    std::vector<bullet*> p2bullets;
    std::vector<bool> p1bool;
    std::vector<bool> p2bool;
    bullet* p1bullet = nullptr;
    bullet* p2bullet = nullptr;

	// instance of BOMB
	std::vector<bomb*> p1bomb_list;
	std::vector<bomb*> p2bomb_list;
	std::vector<bool> p1_bomb_bool;
	std::vector<bool> p2_bomb_bool;
	bomb* p1bomb = nullptr;
	bomb* p2bomb = nullptr;

    bool quit = false;
    SDL_Event event;

  	int PlayerOnecurrentTime = 0;
    int PlayerOnediffTime = 0;
    int PlayerOnelastTime = 0;
    int PlayerOneaccumulator = 0;
    
    int PlayerTwocurrentTime = 0;
    int PlayerTwodiffTime = 0;
    int PlayerTwolastTime = 0;
    int PlayerTwoaccumulator = 0;
  
    //While user does not quit
	bool stay_1_bool = false;
	int stay_1_time = STAY;
	bool stay_2_bool = false;
	int stay_2_time = STAY;
	while( !quit ){
        
        int p1XPos = square1.getXCoord();
        int p1YPos = square1.getYCoord();
        int p2XPos = square2.getXCoord();
        int p2YPos = square2.getYCoord();
      
      	 //cooldown code
        PlayerOnecurrentTime = SDL_GetTicks();
        PlayerOnediffTime = PlayerOnecurrentTime - PlayerOnelastTime;
        PlayerOneaccumulator += PlayerOnediffTime;
        PlayerOnelastTime = PlayerOnecurrentTime;
        
        PlayerTwocurrentTime = SDL_GetTicks();
        PlayerTwodiffTime = PlayerTwocurrentTime - PlayerTwolastTime;
        PlayerTwoaccumulator += PlayerTwodiffTime;
        PlayerTwolastTime = PlayerTwocurrentTime;
        
        //Handle events on queue
        while( SDL_PollEvent(&event) != 0 ){

            //Check for user quit
            if(event.type == SDL_QUIT ){
                quit = true;
            }

            //If the up, down, left, or right arrows keys are used then this is a move event to be handled by square1
            if( event.key.keysym.sym == SDLK_i || event.key.keysym.sym == SDLK_k || event.key.keysym.sym == SDLK_j || event.key.keysym.sym == SDLK_l){
                square1.handleMoveEvent(event);
            }

                //If the w, a, s, or d keys are used then this is a move event to be handled by square2
            else if( event.key.keysym.sym == SDLK_w || event.key.keysym.sym == SDLK_s || event.key.keysym.sym == SDLK_a || event.key.keysym.sym == SDLK_d){
                square2.handleMoveEvent(event);
            }
            
            // bullet, play2, q
            else if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q && PlayerOneaccumulator > 500){
                PlayerOneaccumulator -= 500;
                p2bullet = new bullet(square2.getXCoord(), square2.getYCoord(), &projectile);
                (*p2bullet).handleInput(event, square2.getDirection());
                (*p2bullet).shoot(p2XPos,p2YPos);
                p2bullets.push_back(p2bullet);
                p2bool.push_back(true);
            }

            // bomb, play2, e
            else if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_e && PlayerOneaccumulator > 500){
	            PlayerOneaccumulator -= 500;
	            p2bomb = new bomb(&square2, &projectile, &flameTexture);
	            (*p2bomb).handleInput(event, square2.getDirection());
	            (*p2bomb).launch(p2XPos,p2YPos);
	            p2bomb_list.push_back(p2bomb);
	            p2_bomb_bool.push_back(true);
            }


            else if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_u && PlayerTwoaccumulator > 500){
                PlayerTwoaccumulator -= 500;
                p1bullet = new bullet(square1.getXCoord(), square1.getYCoord(), &projectile);
                (*p1bullet).handleInput(event, square1.getDirection());
                (*p1bullet).shoot(p1XPos,p1YPos);
                p1bullets.push_back(p1bullet);
                p1bool.push_back(true);
            }

            // bomb, play1, o
            else if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_o && PlayerOneaccumulator > 500){
	            PlayerOneaccumulator -= 500;
	            p1bomb = new bomb(&square1, &projectile, &flameTexture);
	            (*p1bomb).handleInput(event, square1.getDirection());
	            (*p1bomb).launch(p1XPos,p1YPos);
	            p1bomb_list.push_back(p1bomb);
	            p1_bomb_bool.push_back(true);
            }
        }

        square1.move();
        square2.move();
      
        if ( checkCollision(square1, square2))
        {
            p1XPos = square1.getXCoord();
            p1YPos = square1.getYCoord();
            p2XPos = square2.getXCoord();
            p2YPos = square2.getYCoord();
            
            int p1newXCoord = p1XPos -= square1.getXMovement();
            int p1newYCoord = p1YPos -= square1.getYMovement();
            int p2newXCoord = p2XPos -= square2.getXMovement();
            int p2newYCoord = p2YPos -= square2.getYMovement();
            
            square1.setXCoord(p1newXCoord);
            square1.setYCoord(p1newYCoord);
            
            square2.setXCoord(p2newXCoord);
            square2.setYCoord(p2newYCoord);
             
        }        
        
        //Clear the screen to remove the players' previous positions
        SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gameRenderer);

        //Render the players in their new positions
        square1.render(gameRenderer);
        square2.render(gameRenderer);
        
        hp1_red.render(gameRenderer);
        hp2_red.render(gameRenderer);
        hp1_green.render(gameRenderer);
        hp2_green.render(gameRenderer);
        
        if (square1.getHealth() <= 0 || square2.getHealth() <= 0) {
            std::cout << "Player 1: " << square1.getHealth() << std::endl;
            std::cout << "Player 2: " << square2.getHealth() << std::endl;
            exit(0);
        }
        
        // Loops through all of player 1's bullets and updates each one individually 
        for (int i = 0; i < (int)p1bullets.size(); i++) {
            if( p1bool[i] ){
                (*p1bullets[i]).shoot((*p1bullets[i]).getX(), (*p1bullets[i]).getY());
                
                if ((*p1bullets[i]).getY() < 0 || (*p1bullets[i]).getY() + (*p1bullets[i]).getHeight() > SCREEN_HEIGHT || projectileCollision(square2, p1bullets[i])){
                    p1bool[i] = false;
                    
                    if (projectileCollision(square2, p1bullets[i])) {
                        square2.setHealth(square2.getHealth() - damage);
                        std::cout << "Player 2 took 10 damage: " << square2.getHealth() << std::endl;
                        hp1_texture.setWidth(hp1_texture.getHpWidth()  - 25);
                    }
                }
                
                else if ((*p1bullets[i]).getX() < 0 || (*p1bullets[i]).getX() + (*p1bullets[i]).getWidth()  > SCREEN_WIDTH || projectileCollision(square2, p1bullets[i])){
                    p1bool[i] = false;
                    
                    if (projectileCollision(square2, p1bullets[i])) {
                        square2.setHealth(square2.getHealth() - damage);
                        std::cout << "Player 2 took 10 damage: " << square2.getHealth() << std::endl;
                        hp1_texture.setWidth(hp1_texture.getHpWidth() - 25);
                        std::cout << "ERROR" << std::endl;
                    }
                }
                
                if( !p1bool[i] ){
                    delete p1bullets[i];
                    p1bullets[i] = nullptr;
                }
                
                else{
                    (*p1bullets[i]).render(gameRenderer);
                }
            }
        }
        
        // Loops through all of player 2's bullets and updates each one individually 
        for (int i = 0; i < (int)p2bullets.size(); i++) {
            if( p2bool[i] ){
                (*p2bullets[i]).shoot((*p2bullets[i]).getX(), (*p2bullets[i]).getY());
                
                if ((*p2bullets[i]).getY() < 0 || (*p2bullets[i]).getY() + (*p2bullets[i]).getHeight() > SCREEN_HEIGHT || projectileCollision(square1, p2bullets[i])){
                    p2bool[i] = false;
                    
                    if (projectileCollision(square1, p2bullets[i])) {
                        square1.setHealth(square1.getHealth() - damage);
                        std::cout << "Player 1 took 10 damage: " << square1.getHealth() << std::endl;
                        hp2_texture.setWidth(hp2_texture.getHpWidth()  - 25);
                    }

                }
                else if ((*p2bullets[i]).getX() < 0 || (*p2bullets[i]).getX() + (*p2bullets[i]).getWidth()  > SCREEN_WIDTH || projectileCollision(square1, p2bullets[i])){
                    p2bool[i] = false;
                    
                    if (projectileCollision(square1, p2bullets[i])) {
                        square1.setHealth(square1.getHealth() - damage);
                        std::cout << "Player 1 took 10 damage: " << square1.getHealth() << std::endl;
                        hp2_texture.setWidth(hp2_texture.getHpWidth()  - 25);
                    }

                }
                
                if(!p2bool[i]){
                    delete p2bullets[i];
                    p2bullets[i] = nullptr;
                }
                
                else{
                    (*p2bullets[i]).render(gameRenderer);
                }
            }
        }

		// loop BUMB p1
		int x_1, y_1;
		if(stay_1_bool && stay_1_time > 0)
		{
			(flameTexture).render(x_1, y_1-150, gameRenderer);
			stay_1_time--;
		} if(stay_1_time == 0)
		{
			stay_1_bool = false; stay_1_time = STAY;
		}
		for (int i = 0; i < (int)p1_bomb_bool.size(); i++) {
			if( p1_bomb_bool[i] ){
				// in bumb, largest flying distance is MAX distance
				if((*p1bomb_list[i]).launch((*p1bomb_list[i]).getX(), (*p1bomb_list[i]).getY()) == 0)
				{
					p1_bomb_bool[i] = false;
					if (projectileExploded(square2, p1bomb_list[i])) {
						square2.setHealth(square2.getHealth() - bomb_damage);
						std::cout << "Player 2 took " << bomb_damage << " damage: " << square2.getHealth() << std::endl;
						hp1_texture.setWidth(hp1_texture.getHpWidth()  - 25 * 3);
					}
				}

				if (((*p1bomb_list[i]).getY() < 0 || (*p1bomb_list[i]).getY() + (*p1bomb_list[i]).getHeight() > SCREEN_HEIGHT)
				    || ((*p1bomb_list[i]).getX() < 0 || (*p1bomb_list[i]).getX() + (*p1bomb_list[i]).getWidth()  > SCREEN_WIDTH)){
					p1_bomb_bool[i] = false;
					if (projectileExploded(square1, p1bomb_list[i])) {
						square2.setHealth(square2.getHealth() - bomb_damage);
						std::cout << "Player 2 took " << bomb_damage << " damage: " << square2.getHealth() << std::endl;
						hp1_texture.setWidth(hp1_texture.getHpWidth()  - 25 * 3);
					}
				}

				if( !p1_bomb_bool[i] ){
					// it means you need to put the center of the graph in the x, y not the left up corner
					int modification_of_render = p1bomb_list[i]->getRange();
					x_1 = p1bomb_list[i]->getX() - modification_of_render; y_1 = p1bomb_list[i]->getY();
					stay_1_bool = true;
					delete p1bomb_list[i];
				}

				if(p1_bomb_bool[i]){
					(*p1bomb_list[i]).render(gameRenderer);
				}
			}
		}

	    // loop BUMB p2
	    int x_2, y_2;
	    if(stay_2_bool && stay_2_time > 0)
	    {
		    (flameTexture).render(x_2, y_2, gameRenderer);
		    stay_2_time--;
	    } if(stay_2_time == 0)
		{
			stay_2_bool = false; stay_2_time = STAY;
		}
	    for (int i = 0; i < (int)p2_bomb_bool.size(); i++) {
		    if( p2_bomb_bool[i] ){
			    // in bumb, largest flying distance is MAX distance
			    if((*p2bomb_list[i]).launch((*p2bomb_list[i]).getX(), (*p2bomb_list[i]).getY()) == 0)
			    {
				    p2_bomb_bool[i] = false;
				    if (projectileExploded(square1, p2bomb_list[i])) {
					    square1.setHealth(square1.getHealth() - bomb_damage);
					    std::cout << "Player 1 took " << bomb_damage << " damage: " << square1.getHealth() << std::endl;
					    hp2_texture.setWidth(hp2_texture.getHpWidth()  - 25 * 3);
				    }
			    }

			    if (((*p2bomb_list[i]).getY() < 0 || (*p2bomb_list[i]).getY() + (*p2bomb_list[i]).getHeight() > SCREEN_HEIGHT)
			        || ((*p2bomb_list[i]).getX() < 0 || (*p2bomb_list[i]).getX() + (*p2bomb_list[i]).getWidth()  > SCREEN_WIDTH)){
				    p2_bomb_bool[i] = false;
				    if (projectileExploded(square1, p2bomb_list[i])) {
					    square1.setHealth(square1.getHealth() - bomb_damage);
					    std::cout << "Player 1 took " << bomb_damage << " damage: " << square1.getHealth() << std::endl;
					    hp2_texture.setWidth(hp2_texture.getHpWidth()  - 25 * 3);
				    }
			    }

			    if( !p2_bomb_bool[i] ){
				    int modification_of_render =  p2bomb_list[i]->getRange();
			    	x_2 = p2bomb_list[i]->getX() - modification_of_render/2; y_2 = p2bomb_list[i]->getY()-modification_of_render/2;
			    	stay_2_bool = true;
				    delete p2bomb_list[i];
			    }

			    if(p2_bomb_bool[i]){
				    (*p2bomb_list[i]).render(gameRenderer);
			    }
		    }
	    }

        //Update the game screen
        SDL_RenderPresent(gameRenderer);
    }

    oneTexture.free();
    twoTexture.free();
    projectile.free();
    hp_red_texture.free();
    hp1_texture.free();
    hp2_texture.free();
}

int game::getScrnHeight(){
    return SCREEN_HEIGHT;
}

int game::getScrnWidth(){
    return SCREEN_WIDTH;
}

// Checks if there is a collision between a player and a bullet
bool game::projectileCollision(player p, bullet* b) {
	return p.getXCoord() <= b->getX() + b->getWidth() && // Checks if player is to the left of bullet
	       p.getXCoord() + p.getWidth() >= b->getX() && // Checks if player is to the right of bullet
	       p.getYCoord() <= b->getY() + b->getHeight() &&
	       p.getYCoord() + p.getHeight() >= b->getY();

}

// Checks if there is a collision between a player and a bullet
bool game::projectileExploded(player p, bomb* b) {
	std::cout << ((b->getX() - p.getXCoord())*(b->getX() - p.getXCoord()) + (b->getY() - p.getYCoord())*(b->getY() - p.getYCoord())) << std::endl;
	std::cout << b->getRange()*b->getRange() << std::endl;
	return ((b->getX() - p.getXCoord())*(b->getX() - p.getXCoord()) + (b->getY() - p.getYCoord())*(b->getY() - p.getYCoord()))
	<= b->getRange()*b->getRange();
}

/**
 * starts execution of game
 * @param argc
 * @param args
 * @return 0 if succeeds
 */
int main( int argc, char* args[] ){

	game* currentGame = nullptr;
    currentGame = (*currentGame).getInstance();

    if( !(*currentGame).init() ){
        std::cout << "Failed to initialize, program is exiting.\n";
    }

    else{
        (*currentGame).run();
    }

    delete currentGame;
    return 0;
}

