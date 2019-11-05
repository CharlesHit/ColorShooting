//
// Created by Christopher Xu on 2019-10-26.
//
/* Code by: Group 16
 * Date: October 24, 2019
 */

#ifndef TEAM16_PLAYER_H
#define TEAM16_PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "playerTexture.h"

class player{

public:
    player(int startX, int startY, playerTexture* pTxt, int HP);
    void handleMoveEvent(SDL_Event& event);
    void move();
    void render(SDL_Renderer*& rend);
    int getXCoord();
    int getYCoord();
    int getXMovement();
    int getYMovement();
    int getDirection();
    int getWidth();
    int getHeight();
    void setXCoord(int x);
  	void setYCoord(int y);
    int getHealth();
    void setHealth(int h);

private:
    int width;
    int height;
    int health;
    int cooldown;
    int velocity = 1;
    int xPosition;
    int yPosition;
    int xMovement;
    int yMovement;
    int direction;
    playerTexture* pTexture;
};

#endif //TEAM16_PLAYER_H
