#include "Bullet.h"

Bullet::Bullet()
{
    xPos = 0;
    yPos = 0;

    direction = 0; //1 = up, 2 = right, 3 = down, 4 = left

    active = false;

    hitBox = new SDL_Rect;
    hitBox->w = 6;
    hitBox->h = 6;
    hitBox->x = 0;
    hitBox->y = 0;
}

Bullet::~Bullet()
{
    hitBox = NULL;
}

void Bullet::restart(int x, int y, char drction, SDL_RendererFlip flip) //so this like places the bullets in their starting position right after they get shot out of the gun? based on where the end of the barrel is which is based off of which direction the gun is???
{
    direction = drction;

    switch ( direction )
    {
        case 1:
            if(flip == SDL_FLIP_NONE)
            {
                hitBox->x = x - 9;
                hitBox->y = y - 35;
            }
            else
            {
                hitBox->x = x + 32;
                hitBox->y = y - 35;
            }
            break;
        case 2:
            hitBox->x = x + 67;
            hitBox->y = y + 59;
            break;
        case 3:
            hitBox->x = x + 30;
            hitBox->y = y - 150;
            break;
        case 4:
            hitBox->x = x - 42;
            hitBox->y = y + 59;
            break;
    }

    active = true;
}

void Bullet::move(float timestep)
{
    if(active)
    {
        switch ( direction )
    {
        case 1:
            hitBox->y -= speed * timestep;
            break;
        case 2:
            hitBox->x += speed * timestep;
            break;
        case 3:
            hitBox->y += speed * timestep;
            break;
        case 4:
            hitBox->x -= speed * timestep;
            break;
    }
    }

    xPos = hitBox->x;
    yPos = hitBox->y;
}
