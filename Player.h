#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED

#include <SDL.h>
#include "Texture.h"
#include "Timer.h"

#include <iostream>

class Player
{
    public:
		Player();
		~Player();

		short int speed = 250;

		void handleEvent( SDL_Event& event );

		void move( float timeStep );
		void update();

		void render(SDL_Renderer* renderer, SDL_Texture* texture);

		float xPos; //coordinates of player graphic
		float yPos; //coordinates of player graphic
		float velocity; //useless number so the hitbox doesn't sperg out
		float yVelocity; //useless number so the hitbox doesn't sperg out

		Timer jumpTimer;
		void jump(float timeStep);
		bool jumping;
		bool released;
		bool onFlatSurface;
		bool foo;
        bool bar;
        bool baz;
        bool cantGoRight;
        bool cantGoLeft;
        bool goinLeft;
		bool goinRight;

		bool hasGun;

        short int gunDirection;
        bool holdingUp;
        Timer gunTimer;
        short int bullet;
        bool shoot;
        int clip;
        int clipSize;
        int reserve;
        void reload();
        Timer reloadTimer;

        bool uncrouch;
        bool canUncrouch;
        bool crouched; //only used for animations for now

        bool onZipline;

        short int animation;
        short int getAnimation();
        SDL_RendererFlip flip;

        SDL_Rect* hitBox;
		SDL_Rect* rectangleForTheHair;

		bool secretUnlocked;
};

#endif // CHARACTER_H_INCLUDED
