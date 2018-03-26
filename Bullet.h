#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#include <SDL.h>

class Bullet
{
    public:
        Bullet();
        ~Bullet();

        static const short int speed = 770;

        float xPos;
        float yPos;

        char direction; //1 = up, 2 = right, 3 = down, 4 = left

        bool active;

        SDL_Rect* hitBox;

        void restart(int x, int y, char direction, SDL_RendererFlip flip);

        void move(float timestep);
};


#endif // BULLET_H_INCLUDED
