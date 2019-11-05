//
// Created by Christopher Xu on 2019-10-26.
//

#ifndef TEAM16_BULLET_H
#define TEAM16_BULLET_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>
#include "player.h"

class bullet{

public:
    bullet(int startX, int startY,playerTexture* pTxt);
    ~bullet();
    void handleInput(SDL_Event& event, int direction);
    void shoot(int xPos, int yPos);
    void render(SDL_Renderer*& rend);
    int getX();
    int getY();
    int getHeight();
    int getWidth();

private:
    int width;
    int height;
    int velocity = 2;
    int xPosition;
    int yPosition;
    int xMovement;
    int yMovement;
    playerTexture* pTexture;
};

#endif //TEAM16_BULLET_H
