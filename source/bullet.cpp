//
// Created by Christopher Xu, Ivgeni Darinski, Nicholas Meisner, Mu He and Xinwen Liang  on 2019-10-26.
//
//
//

#include "bullet.h"
#include "game.h"

/**
 * This function initializes an object of class bullet (projectiles used in the game)
 * @return bullet object
 * @param xPos
 * @param yPos
 * @param pTxt
 */
bullet::bullet(int xPos, int yPos,playerTexture* pTxt)
{
    pTexture = pTxt;
    width = (*pTexture).getTxtWidth();
    height = (*pTexture).getTxtWidth();
    xPosition = xPos;
    yPosition = yPos;
    xMovement = 0;
    yMovement = 0;
}

/**
 * Destructor for the bullet class
 */
bullet::~bullet() = default;

/**
 *
 * This function determines velocity of bullet
 * @param event - user input
 * @param direction - direction that the player has moved (determined bullet direction)
 */
void bullet::handleInput(SDL_Event& event, int direction){

    //Checks if the event is a button being pushed down and prevents key repeats, have to move/render new position before handling another event
    if(event.type == SDL_KEYDOWN && event.key.repeat == 0){

        if((event.key.keysym.sym == SDLK_q || event.key.keysym.sym == SDLK_u) && direction == 0){
            yMovement = yMovement - velocity; //Move up
        }

        else if((event.key.keysym.sym == SDLK_q || event.key.keysym.sym == SDLK_u) && direction == 1){
            yMovement = yMovement + velocity; //Move down
        }

        else if((event.key.keysym.sym == SDLK_q || event.key.keysym.sym == SDLK_u) && direction == 2){
            xMovement = xMovement - velocity; //Move left
        }

        else if((event.key.keysym.sym == SDLK_q || event.key.keysym.sym == SDLK_u) && direction == 3){
            xMovement = xMovement + velocity; //Move right
        }
    }

}

/**
 *
 * the function changes the coordinates of the bullet based on the handleInput() function
 * @param xPos
 * @param yPos
 */
void bullet::shoot(int xPos, int yPos){
    
    yPosition = yPos + yMovement;

    xPosition = xPos + xMovement;
}

/**
 *
 * This function renders the bullet based on the coordinates of the bullets
 * @param rend - texture object of bullet
 */
void bullet::render(SDL_Renderer*& rend){
    (*pTexture).render(xPosition, yPosition, rend);
}

/**
 *
 * @return xPosition - x coordinate of bullet
 */
int bullet::getX(){
    return xPosition;
}

/**
 *
 * @return yPosition - y coordinate of bullet
 */
int bullet::getY(){
    return yPosition;
}

/**
 *
 * @return width - width of bullet
 */
int bullet::getWidth(){
    return width;
}

/**
 *
 * @return height - height of bullet
 */
int bullet::getHeight(){
    return height;
}



