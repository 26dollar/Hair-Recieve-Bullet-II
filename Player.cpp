#include "Player.h"

Player::Player()
{
    speed = 250;

    xPos = 10;
    yPos = 10;

    hitBox = new SDL_Rect;
    hitBox->w = 30;
    hitBox->h = 121;
    hitBox->x = 10;
    hitBox->y = 10;

    rectangleForTheHair = new SDL_Rect;
    rectangleForTheHair->w = 30;
    rectangleForTheHair->h = 15;
    rectangleForTheHair->x = 10;
    rectangleForTheHair->y = 10;

    jumping = false;
    released = false;
    onFlatSurface = false;
    goinLeft = false;
    goinRight = false;
    foo = false;
    bar = false;
    cantGoRight = false;

    hasGun = false;
    gunDirection = 2;
    holdingUp = false;

    velocity = 50;
    yVelocity = 50;

    bullet = -1;
    shoot = false;

    uncrouch = false;
    canUncrouch = true;
    crouched = false;

    clip = 0;
    clipSize = 15;
    reserve = 900;

    animation = 0;
    flip =  SDL_FLIP_NONE;

    onZipline = false;
    secretUnlocked = false;
}

Player::~Player()
{
    hitBox = NULL;
	rectangleForTheHair = NULL;
}

void Player::handleEvent( SDL_Event& event )
{
    if( event.type == SDL_KEYDOWN && event.key.repeat == 0 ) //key pressed
    {
        switch( event.key.keysym.sym )
        {
            case SDLK_LEFT:
                {
                    if(onZipline)
                    {
                        goinLeft = true;
                        break;
                    }
                    else
                    {
                        goinLeft = true;
                        gunDirection = 4;
                        flip = SDL_FLIP_HORIZONTAL;
                    }
                } break;

            case SDLK_RIGHT:
                {
                    if(onZipline)
                    {
                        goinRight = true;
                        break;
                    }
                    else
                    {
                        goinRight = true;
                        gunDirection = 2;
                        flip = SDL_FLIP_NONE;
                    }
                } break;

            case SDLK_z:
                {
                    if(onFlatSurface)
                    {
                        jumping = true;
                        released = false;
                    }
                } break;

            case SDLK_x:
                {
                    if(reserve > 0 && clip < 1 && hasGun)
                    {
                        reloadTimer.start();
                    }

                    if(!gunTimer.isStarted() && clip > 0 && hasGun)
                    {
                        bullet++; gunTimer.start();
                        shoot = true;
                    }
                } break;

            case SDLK_DOWN:
                {
                    if(onFlatSurface)
                    {
                        yVelocity += 10;
                    }
                    hitBox->h = 91;
                    uncrouch = false;
                    crouched = true;
                } break;
            case SDLK_UP:
                {
                    holdingUp = true;
                } break;
        }
    }

    else if( event.type == SDL_KEYUP && event.key.repeat == 0 ) //key released
    {
        switch( event.key.keysym.sym )
        {
            case SDLK_LEFT: goinLeft = false; break;
            case SDLK_RIGHT: goinRight = false; break;
            case SDLK_z: released = true; break;
            case SDLK_x: { reloadTimer.stop(); }break;
            case SDLK_DOWN:{ uncrouch = true; } break;
            case SDLK_UP: { holdingUp = false; } break;
        }
    }
}

void Player::move( float timeStep)
{
    //move horizontally
    if(goinLeft && !onZipline ) // && !cantGoLeft)
    {
        velocity -= speed * timeStep;
    }

    if(goinRight && !onZipline )//&& !cantGoRight)
    {
        velocity += speed * timeStep;
    }

    //move vertically
    if(!onFlatSurface)
    {
        yVelocity += 6.4; //fall from gravity
    }

    if(jumping)
    {
        jump(timeStep);
    }

    hitBox->x = velocity;
    hitBox->y = yVelocity;
}

void Player::jump(float timeStep)
{
    //do a FUCKing retarded jump that lasts a max of 600ms
    if(!jumpTimer.isStarted())
    {
        jumpTimer.start();
    }

    yVelocity -= 700 * timeStep; //go up


    if(!released) //if the jump key is released you drop like a baby
    {
        yVelocity += jumpTimer.getTicks() / 2 * timeStep;
    }
    else
    {
        yVelocity += jumpTimer.getTicks() / 1 * timeStep;
    }


    if(jumpTimer.getTicks() >= 600)
    {
        jumping = false;
        jumpTimer.stop();
    }

}

void Player::update()
{
                if(!crouched)
                {
                    speed = 250;
                }
                else
                {
                    speed = 120;
                }


                if(foo) //if collision with flat surface detected
                {
                    onFlatSurface = true;
                }
                else
                {
                    onFlatSurface = false;
                }

                /*if(bar) //if collision with left wall detected
                {
                    cantGoRight = true;
                }
                else
                {
                    cantGoRight = false;
                }

                if(baz) //if collision with right wall detected
                {
                    cantGoLeft = true;
                }
                else
                {
                    cantGoLeft = false;
                }*/

                if(uncrouch)
                {
                    if(onFlatSurface && canUncrouch)
                    {
                        yVelocity -= 30;
                        hitBox->h = 121;
                        uncrouch = false;
                        crouched = false;
                    }
                    else if(onZipline)
                    {
                       hitBox->h = 121;
                       uncrouch = false;
                       crouched = false;
                    }
                }


                hitBox->x = velocity;
                hitBox->y = yVelocity;
                xPos = hitBox->x - 18;
                yPos = hitBox->y - 16;
                rectangleForTheHair->x = hitBox->x;
                rectangleForTheHair->y = hitBox->y - 15;

                foo = false;
                bar = false;
                baz = false;

                shoot = false;
                if(gunTimer.getTicks() >= 150) //have to wait 100ms until ye can shoot
                {
                    gunTimer.stop();
                }
                if(bullet >= 18)
                {
                    bullet = -0;
                }
                if(reloadTimer.getTicks() > 1530) //1530ms reload time
                {
                    reload();
                }

                if(holdingUp)
                {
                    gunDirection = 1;
                }
                else if(flip == SDL_FLIP_NONE)
                {
                    gunDirection = 2;
                }
                else
                {
                    gunDirection = 4;
                }

                if(gunDirection == 1) //can't reload when you're pointing up
                {
                    reloadTimer.stop();
                }
}

void Player::reload()
{
    reloadTimer.stop();

    while(clip < clipSize)
    {
        reserve--;
        clip++;
        if(reserve < 1)
        {
            return;
        }
    }

}

short int Player::getAnimation()
{
    if(!onFlatSurface)
    {
        if(!crouched)
        {
            animation = 3;
            return  3;
        }
        else
        {
            animation = 4;
            return  4;
        }
    }
    else if(crouched)
    {
        if(goinLeft || goinRight)
        {
            animation = 5;
            return  5;
        }
        else
        {
            animation = 4;
            return  4;
        }
    }
    else if(onFlatSurface && goinLeft || goinRight)
    {
        animation = 2;
        return 2;
    }

    else
    {
        animation = 0;
        return 0;
    }
}
