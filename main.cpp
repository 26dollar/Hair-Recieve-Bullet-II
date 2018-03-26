#include <iostream>
#include <sstream>
#include <iomanip>

#include <stdio.h>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "Graphics.h"
#include "Texture.h"
#include "Player.h"
#include "Timer.h"
#include "FlatSurface.h"
#include "Bullet.h"
#include "Zipline.h"


//TTF_Font *gFont = NULL;

const int SCREEN_FPS = 60;
const int SCREEN_TICK_PER_FRAME = 1000.f / SCREEN_FPS;

Graphics window;

///WHO'S CALLING MY CODE SHADY! I PITY THE FOOL WHO'S CALLIN' 26'S CODE SHADY! PITY I SAY!


//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Scene textures
Texture gPlayer(65, 136);
Texture gPlayerPixelated(65, 136);
Texture gPlayerArmed(65, 136);
Texture gPlayerSemi;
Texture gGround;
Texture gTestBox;
Texture ammo1(53, 59);
Texture ammo2;
Texture gWall;
Texture gTarget;
Texture gBullet;
Texture gHair(86, 33);
Texture gSemi_Auto;
Texture gZipline;
Texture gZiplineBase;
Texture gTutorialZipline1;
Texture gDemoDay;

Texture gBackground;

/*double getTimeScalar() ///best function  ///this isnt even used at all anymoroe and never will be and it didnt work
{
    static double lastticks = 0;

    double ticks = SDL_GetTicks();
    double tickssince = ticks - lastticks;

    double scalar = tickssince/1000;

    lastticks = ticks;

    std::cout << std::setprecision(14) << scalar << std::endl;

   return scalar;
}*/

int checkCollision( SDL_Rect* a, SDL_Rect* b, int mode )
{
    ///mode 1 = normal collision detection
    ///mode 2 = patented 4 side box collisionm [broken]
    ///mode 3 = only checks top side
    ///mode 4 = unpatented 4 side box collision beta [surprisingly less broken?]
    ///mode 5 = only check right side
    ///mode 6 = only check left side

    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a->x;
    rightA = a->x + a->w;
    topA = a->y;
    bottomA = a->y + a->h;

    //Calculate the sides of rect B
    leftB = b->x;
    rightB = b->x + b->w;
    topB = b->y;
    bottomB = b->y + b->h;

    if(mode == 1)
    {
        if( bottomA <= topB )
        {
            return false;
        }

        if( topA >= bottomB )
        {
            return false;
        }

        if( rightA <= leftB )
        {
            return false;
        }

        if( leftA >= rightB )
        {
            return false;
        }
        return 1; //true, collided
    }
    else if (mode == 4)
    {
        if( rightA < rightB && bottomA > topB && rightA > leftB && topA < bottomB && rightA < leftB + 10) //if pushing right through left side
        {
            return 2; //needs pushed left
        }                                   // + ?
        else if( leftA > leftB &&  bottomA > topB && leftA < rightB && topA < bottomB && leftA > rightB - 10 ) //if pushing left through right side
        {
            return 3; //needs pushed right
        }
        else if( topA < topB && bottomA > topB && rightA > leftB && leftA < rightB) //if falling through top
        {
            return 1; //needs pushed up
        }                                                   ///make this number bigger if shit gets weird vvvvvv
        else if(rightA > leftB && leftA < rightB && topA < bottomB && bottomA > bottomB) //if pushing up through bottom
        {
            return 4; //needs pushed down
        }                                    // + ?
    }
    else if(mode == 3)
    {
        if( topA < topB && bottomA >= topB && rightA > leftB && leftA < rightB && bottomA < topB + 20) //if falling through top
        {
            return 1; //needs pushed up
        }
    }
    else if(mode == 2)
    {                                             ///make this number bigger if shit gets weird vvvvvv
        if( topA < topB && bottomA > topB && rightA > leftB && leftA < rightB && bottomA < topB + 14) //if falling through top
        {
            return 1; //needs pushed up
        }                                                   ///make this number bigger if shit gets weird vvvvvv
        else if(rightA > leftB && leftA < rightB && topA < bottomB && bottomA > bottomB && topA > bottomB - 14) //if pushing up through bottom
        {
            return 4; //needs pushed down
        }                                    // + ?
        else if( rightA < rightB && bottomA > topB && rightA > leftB && topA < bottomB ) //if pushing right through left side
        {
            return 2; //needs pushed left
        }                                   // + ?
        else if( leftA > leftB &&  bottomA > topB && leftA < rightB && topA < bottomB ) //if pushing left through right side
        {
            return 3; //needs pushed right
        }
    }
    else if (mode == 5)
    {
        if( leftA > leftB &&  bottomA > topB && leftA < rightB && topA < bottomB ) //if pushing left through right side
        {
            return 1; //needs pushed right
        }
    }
    else if (mode == 6)
    {
        if( rightA < rightB && bottomA > topB && rightA > leftB && topA < bottomB ) //if pushing right through left side
        {
            return 1; //needs pushed left
        }
    }
    return 5; //do nothing
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

    if( !gPlayer.loadFile("boardgame/player1_unarmed.map", window.gRenderer) )
    {  printf("Failed to load player1spritesheet.png");
        success = false;
    }

    if( !gPlayerPixelated.loadFile("boardgame/test2_tvsize.map", window.gRenderer) )
    {  printf("Failed to load test2_tvsize.png");
        success = false;
    }

    if( !gPlayerArmed.loadFile("boardgame/hold_gun.map", window.gRenderer) )
    {  printf("Failed to load hold_gun.png");
        success = false;
    }

    if( !gPlayerSemi.loadFile("boardgame/semi_auto_sheet_test2.map", window.gRenderer) )
    {  printf("Failed to load semi_auto_sheet_test2.png");
        success = false;
    }

    if( !ammo1.loadFile("boardgame/delete_this.map", window.gRenderer) )
    {
        printf("Failed to load delete_this.png");
        success = false;
    }

    if( !ammo2.loadFile("boardgame/delete_this2.map", window.gRenderer) )
    {
        printf("Failed to load delete_this2.png");
        success = false;
    }

    if( !gBackground.loadFile("boardgame/tfijaf3.map", window.gRenderer) )
    {
        printf("Failed to load tfijaf.png");
        success = false;
    }

    if( !gGround.loadFile("boardgame/ground.map", window.gRenderer) )
    {
        printf("Failed to load ground.png");
        success = false;
    }

    if( !gWall.loadFile("boardgame/wall.map", window.gRenderer) )
    {
        printf("Failed to load wa;;.png");
        success = false;
    }

    if( !gTarget.loadFile("boardgame/target.map", window.gRenderer) )
    {
        printf("Failed to load teargpng");
        success = false;
    }

    if( !gTestBox.loadFile("boardgame/testbox.map", window.gRenderer) )
    {
        printf("Failed to load testbox.png");
        success = false;
    }

    if( !gBullet.loadFile("boardgame/bullet_round.map", window.gRenderer) )
    {
        printf("Failed to load bejjytr.png");
        success = false;
    }

    if( !gHair.loadFile("boardgame/hair_black.map", window.gRenderer) )
    {  printf("Failed to load hair.png");
        success = false;
    }

    if( !gSemi_Auto.loadFile("boardgame/semi_auto2.map", window.gRenderer) )
    {  printf("Failed to load semi_auto.png");
        success = false;
    }

    if( !gZipline.loadFile("boardgame/zipline_spritesheet_test.map", window.gRenderer) )
    {  printf("Failed to load zipline_spritesheet.png");
        success = false;
    }

    if( !gZiplineBase.loadFile("boardgame/useless_wire_delete_this.map", window.gRenderer) )
    {  printf("Failed to load useless_aesthetic_wire_delete_this.png");
        success = false;
    }

    if( !gTutorialZipline1.loadFile("boardgame/tutorialzipline1.map", window.gRenderer) )
    {  printf("Failed to load tutorialzipline1.png");
        success = false;
    }

    if( !gDemoDay.loadFile("boardgame/demoday.png", window.gRenderer) )
    {  printf("Failed to load demoday.png");
        success = false;
    }

    //Open the font
	/*gFont = TTF_OpenFont( "boardgame/FreeSans.ttf", 28 );
	if( gFont == NULL )
	{
		printf( "Failed to load FreeSans.ttf SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}*/

	return success;
}

void close()
{
	//Free loaded images
	gPlayer.free();
	gPlayerPixelated.free();
	gPlayerArmed.free();
	gPlayerSemi.free();
	gGround.free();
	gWall.free();
	gTestBox.free();
	gBackground.free();
    gTarget.free();
    gBullet.free();
    gHair.free();
    gSemi_Auto.free();
	ammo1.free();
	ammo2.free();
	gZipline.free();
	gZiplineBase.free();
	gDemoDay.free();
	gTutorialZipline1.free();

	//Destroy window
	SDL_DestroyRenderer( window.gRenderer );
	SDL_DestroyWindow( window.gWindow );
	window.gWindow = NULL;
	window.gRenderer = NULL;

	//TTF_CloseFont( gFont );
	//gFont = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
	//TTF_Quit();
}

int main( int argc, char* args[] )
{
    SDL_Rect* camera = new SDL_Rect{ 0, 0, 960, 540 };
    Player player1;
    FlatSurface ground(10, 450, 1400, 50, 0, 447);
    FlatSurface ground2(2245, 845, 1400, 50, 2235, 843);
    FlatSurface leftWall(0, 0, 20, 450, 0, 0);
    FlatSurface target(1225, 315, 20, 10, 1220, 289);
    FlatSurface Secrettarget(-350, 300, 30, 30, -325, 297);
    FlatSurface semi_auto(1000, 425, 20, 20, 980, 425);

    FlatSurface testBox[3] =
    {
        FlatSurface(700, 30, 100, 90, 700, 30),
        FlatSurface(650, 360, 100, 90, 650, 360),
        FlatSurface(700, 270, 100, 90, 700, 270)
    };

    Bullet player1bullets[20];

    Zipline zipline1(1372, 200);

	//Start up SDL and create window
	if( !window.Initialize() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;



            int countedFrames = 0;

            SDL_Color textColor = { 0, 0, 0, 255 };

            //std::stringstream timeText; ///entirely useless stringstream. don't delete it or everything will break for NO REASON. ,nvm, fixed

			//The frames per second timer
            Timer fpsTimer;
            //The frames per second cap timer
            Timer capTimer;

            fpsTimer.start();

            double timeStep = 0;

            ///FUCK
            double last = 0;
            double now = 0;
            double passed = 0;
            double accumulator = 0;
            double dt = 0;

			//While application is running
			while( !quit )
			{
			    now = SDL_GetTicks();
			    passed = now - last;
			    last = now;
			    accumulator += passed;

			    capTimer.start();

        while (accumulator >= dt) {

				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}

					player1.handleEvent( e );

				}

				//player1.hitBox->y = 90;

				/*********TIME SHIT*********/
                //Calculate and correct fps
                float avgFPS = countedFrames / ( fpsTimer.getTicks() / 1000.f );
                if( avgFPS > 2000000 )
                {
                        avgFPS = 0;
                }

				//Calculate time step
				//timeStep = getTimeScalar();
				dt = 1000 / 60;
				timeStep = 0.016;
                /*********TIME SHIT*********/

                /********* everything *********/

                if(player1.shoot  && player1.clip > 0)
                {
                    player1bullets[player1.bullet].restart(player1.hitBox->x, player1.hitBox->y, player1.gunDirection, player1.flip);
                    player1.clip--;
                }

                player1.move( timeStep );

                for(int i = 0; i < 19; i++)
                {
                    player1bullets[i].move( timeStep );
                }
                zipline1.move( timeStep );


                if(player1.hitBox->y > 1000)
                {
                    player1.velocity = 50;
                    player1.yVelocity = 50;
                    zipline1.docked = true;
                    zipline1.xVelocity = 1372;
                    zipline1.yVelocity = 200;
                }

                ///shady ass collision code
                //shady ass collision code
                ///shady ass collision code
                for(int i = 0; i < 3; i++)
                {
                    switch ( checkCollision(player1.hitBox, testBox[i].hitBox, 4) ) ///collision for player hitting test boxes
                    {
                        case 1: player1.yVelocity = testBox[i].hitBox->y - player1.hitBox->h + 1;
                                player1.foo = true;
                                if(player1.jumpTimer.getTicks() > 40) {player1.jumping = false; player1.jumpTimer.stop();}
                            break;                                                        //+1?
                        case 2: player1.velocity = testBox[i].hitBox->x - player1.hitBox->w;
                                player1.bar = true;
                            break;
                        case 3: player1.velocity = testBox[i].hitBox->x + testBox[i].hitBox->w;
                                player1.baz = true;
                            break;
                        case 4: player1.yVelocity = testBox[i].hitBox->y + testBox[i].hitBox->h;
                                player1.released = true;
                            break;
                        case 5: //do nothing

                            break;
                    }
                }

                if( checkCollision(player1.rectangleForTheHair, testBox[2].hitBox, 1) == 1) ///collision for hair hitbox dealing with uncrouching
                {
                    player1.canUncrouch = false;
                }
                else
                {
                    player1.canUncrouch = true;
                }

                for(int i = 0; i < 19; i++)
                {
                    if(player1bullets[i].active)
                    {
                        for(int a = 0; a < 3; a++)
                        {
                            if(checkCollision(player1bullets[i].hitBox, testBox[a].hitBox, 1) == 1)
                            {
                                player1bullets[i].active = false;
                            }
                        }
                    }
                }

                if(checkCollision(player1.hitBox, ground.hitBox, 3) == 1)       ///collision for player and the groundseses, walls, and GUN
                {
                    player1.yVelocity = ground.hitBox->y - player1.hitBox->h +1;
                    player1.foo = true;
                }

                if(checkCollision(player1.hitBox, ground2.hitBox, 3) == 1)
                {
                    player1.yVelocity = ground2.hitBox->y - player1.hitBox->h +1;
                    player1.foo = true;
                }

                if(checkCollision(player1.hitBox, leftWall.hitBox, 5) == 1)
                {
                    player1.velocity = leftWall.hitBox->x + leftWall.hitBox->w;
                }

                if(checkCollision(player1.hitBox, semi_auto.hitBox, 1) == 1)
                {
                    player1.secretUnlocked = false;
                    player1.hasGun = true;
                }

                for(int i = 0; i < 19; i++) ///collision for player bullets hitting target
                {
                    if(player1bullets[i].active)
                    {
                        for(int a = 0; a < 3; a++)
                        {
                            if(checkCollision(player1bullets[i].hitBox, target.hitBox, 1) == 1)
                            {
                                player1bullets[i].direction = 4;
                            }
                        }
                    }
                }

                for(int i = 0; i < 19; i++) ///collision for player bullets hitting the secret target
                {
                    if(player1bullets[i].active)
                    {
                        for(int a = 0; a < 3; a++)
                        {
                            if(checkCollision(player1bullets[i].hitBox,  Secrettarget.hitBox, 1) == 1)
                            {
                                player1bullets[i].active = false;
                                player1.hasGun = false;
                                player1.secretUnlocked = true;
                            }
                        }
                    }
                }

                for(int i = 0; i < 19; i++) ///collision for player bullets hitting player hair
                {
                    if(player1bullets[i].active)
                    {
                        //for(int a = 0; a < 3; a++) WHY THE FUCK DID IT ORIGINALLY CHECK FOR COLLISION THREE TIMES?
                        ///--ask tony
                        //{s
                            if(checkCollision(player1bullets[i].hitBox, player1.rectangleForTheHair, 1) == 1)
                            {
                                if(player1.clip < 15 && player1bullets[i].active)
                                {
                                    player1.clip++;
                                    gHair.animationTimer1.start();
                                }
                                player1bullets[i].active = false;
                            }
                        //}
                    }
                }

                for(int i = 0; i < 19; i++) ///collision for player bullets hitting zipline
                {
                    if(player1bullets[i].active)
                    {
                        if(checkCollision(player1bullets[i].hitBox, zipline1.hitBox, 1) == 1)
                        {
                            zipline1.powerTimer.start();
                            player1bullets[i].active = false;
                        }
                    }
                }

                std::cout << "dddd: " << gZipline.ziplineTimer.getTicks() / 100 << std::endl;

                if(player1.flip == SDL_FLIP_NONE) ///collision for the zipline and player
                {
                    if(zipline1.xVelocity >= 2284)
                    {
                        zipline1.xVelocity = 2284;
                        zipline1.yVelocity = 656;
                        //zipline1.xPos = 2284;
                        //zipline1.yPos = 656;
                        zipline1.docked = true;
                    }
                    else
                    {
                        zipline1.docked = false;
                    }

                    if(zipline1.xVelocity <= 1372)
                    {
                        zipline1.xVelocity = 1372;
                        zipline1.yVelocity = 200;
                        //zipline1.xPos = 1372;
                        //zipline1.yPos = 200;
                        zipline1.powerTimer.stop();
                        zipline1.docked = true;
                    }

                    player1.rectangleForTheHair->x -= 20;
                    player1.rectangleForTheHair->y -= 25;
                    if( player1.gunDirection == 1 && checkCollision(player1.rectangleForTheHair, zipline1.hitBox, 1) == 1)
                    {
                        if(zipline1.carrying)
                        {
                            player1.onZipline = true;
                            player1.velocity = zipline1.xVelocity + 20;
                            player1.yVelocity = zipline1.yVelocity + 50;
                        }
                        else
                        {
                            zipline1.docked = false;
                            zipline1.carrying = true;
                        }
                    }
                    else
                    {
                        zipline1.carrying = false;
                        player1.onZipline = false;
                    }
                    player1.rectangleForTheHair->x += 20;
                    player1.rectangleForTheHair->y += 25;

                    zipline1.hitBox->x = zipline1.xVelocity;
                    zipline1.hitBox->y = zipline1.yVelocity;
                }
                else
                {
                   if(zipline1.xVelocity >= 2284)
                    {
                        zipline1.xVelocity = 2284;
                        zipline1.yVelocity = 656;
                        zipline1.docked = true;
                    }
                    else
                    {
                        zipline1.docked = false;
                    }

                    if(zipline1.xVelocity <= 1372)
                    {
                        zipline1.xVelocity = 1372;
                        zipline1.yVelocity = 200;
                        zipline1.powerTimer.stop();
                        zipline1.docked = true;
                    }

                    player1.rectangleForTheHair->x += 20;
                    player1.rectangleForTheHair->y -= 25;
                    if( player1.gunDirection == 1 && checkCollision(player1.rectangleForTheHair, zipline1.hitBox, 1) == 1)
                    {

                        if(zipline1.carrying)
                        {
                            player1.onZipline = true;
                            player1.velocity = zipline1.xVelocity - 20;
                            player1.yVelocity = zipline1.yVelocity + 50;
                        }
                        else
                        {
                            zipline1.docked = false;
                            zipline1.carrying = true;
                        }
                    }
                    else
                    {
                        zipline1.carrying = false;
                        player1.onZipline = false;
                    }
                    player1.rectangleForTheHair->x -= 20;
                    player1.rectangleForTheHair->y += 25;
                }
                zipline1.update();


                ///SHADY ASS COLLISION CODE
                ////////////////////////////SHADY ASS COLLISION CODE
                ///SHADY ASS COLLISION CODE

                player1.update();

                camera->x = ( player1.hitBox->x + player1.hitBox->w / 2 ) - camera->w / 2;
                camera->y = ( player1.hitBox->y - player1.hitBox->h / 2 ) - camera->h / 2;

        accumulator -= dt;
        }
				/******** rendering ********/
				SDL_SetRenderDrawColor( window.gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( window.gRenderer );

				gBackground.render(window.gRenderer, 0 , 0);
				gDemoDay.render(window.gRenderer, 30 - camera->x, 60 - camera->y);
				gTutorialZipline1.render(window.gRenderer, 1400 - camera->x, 300 - camera->y);

				if(!player1.hasGun)
                {
                    gSemi_Auto.render(window.gRenderer, semi_auto.xPos - camera->x, semi_auto.yPos - camera->y);
                }

				for(int i = 0; i < 19; i++)
                {
                    if(player1bullets[i].active)
                    {
                        gBullet.render(window.gRenderer, player1bullets[i].xPos - camera->x, player1bullets[i].yPos - camera->y);
                    }
                }

                if(player1.secretUnlocked)
                {
                    gPlayerPixelated.renderPlayer(player1.getAnimation(), window.gRenderer, (int)player1.xPos - camera->x, (int)player1.yPos - camera->y, player1.flip);
                }
				else if(player1.hasGun)
                {
                    gPlayerArmed.renderPlayer(player1.getAnimation(), window.gRenderer, (int)player1.xPos - camera->x, (int)player1.yPos - camera->y, player1.flip);
                    gPlayerSemi.renderSemi(window.gRenderer, player1.xPos - 0 - camera->x, player1.yPos + 72 - camera->y, player1.flip, player1.gunTimer.getTicks(), player1.reloadTimer.getTicks(), gPlayerArmed.gunOffset, player1.gunDirection, player1.animation);
                }
				else
                {
                    gPlayer.renderPlayer(player1.getAnimation(), window.gRenderer, (int)player1.xPos - camera->x, (int)player1.yPos - camera->y, player1.flip);
                }
				gTestBox.render(window.gRenderer, testBox[0].xPos - camera->x, testBox[0].yPos - camera->y );
				gTestBox.render(window.gRenderer, testBox[1].xPos - camera->x, testBox[1].yPos - camera->y );
				gTestBox.render(window.gRenderer, testBox[2].xPos - camera->x, testBox[2].yPos - camera->y );
				gGround.render(window.gRenderer, ground.xPos - camera->x, ground.yPos - camera->y );
				gGround.render(window.gRenderer, ground2.xPos - camera->x, ground2.yPos - camera->y );
				gWall.render(window.gRenderer, leftWall.xPos - camera->x, leftWall.yPos - camera->y);
				gTarget.render(window.gRenderer, target.xPos - camera->x, target.yPos - camera->y);
                gZiplineBase.render(window.gRenderer, 1372 - camera->x, 196 - camera->y);
				gHair.renderHair(window.gRenderer, player1.rectangleForTheHair->x - 28 - camera->x, player1.rectangleForTheHair->y - 10 - camera->y, player1.flip);
				gZipline.renderZipline(window.gRenderer, zipline1.xPos - camera->x, zipline1.yPos - camera->y, zipline1.docked, zipline1.carrying, zipline1.powerTimer.isStarted());

				///RECTANGLE RENDERING
				SDL_SetRenderDrawColor( window.gRenderer, 0, 250, 0, 250 );

				/*player1.hitBox->x -= camera->x;
				player1.hitBox->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, player1.hitBox );
                player1.hitBox->x += camera->x;
				player1.hitBox->y += camera->y;*/

				/*zipline1.hitBox->x -= camera->x;
				zipline1.hitBox->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, zipline1.hitBox );
                zipline1.hitBox->x += camera->x;
				zipline1.hitBox->y += camera->y;*/

				SDL_SetRenderDrawColor( window.gRenderer, 250, 0, 60, 250 );

                SDL_SetRenderDrawColor( window.gRenderer, 0, 0, 250, 250 );

                /*player1.rectangleForTheHair->x -= camera->x;
				player1.rectangleForTheHair->y -= camera->y;

				if(player1.flip == SDL_FLIP_NONE)
                {
                    player1.rectangleForTheHair->x -= 20;
                }
                else
                {
                    player1.rectangleForTheHair->x += 20;
                }
				player1.rectangleForTheHair->y -= 25;
                SDL_RenderDrawRect( window.gRenderer, player1.rectangleForTheHair );
                if(player1.flip == SDL_FLIP_NONE)
                {
                    player1.rectangleForTheHair->x += 20;
                }
                else
                {
                    player1.rectangleForTheHair->x -= 20;
                }
				player1.rectangleForTheHair->y += 25;
                player1.rectangleForTheHair->x += camera->x;
				player1.rectangleForTheHair->y += camera->y;*/

                //SDL_RenderDrawRect( window.gRenderer, testBox[0].hitBox );
                //SDL_RenderDrawRect( window.gRenderer, testBox[1].hitBox );
                //SDL_RenderDrawRect( window.gRenderer, testBox[2].hitBox );
                ground.hitBox->x -= camera->x;
                ground.hitBox->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, ground.hitBox );
                ground.hitBox->x += camera->x;
                ground.hitBox->y += camera->y;

                target.hitBox->x -= camera->x;
                target.hitBox->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, target.hitBox );
                target.hitBox->x += camera->x;
                target.hitBox->y += camera->y;

                Secrettarget.hitBox->x -= camera->x;
                Secrettarget.hitBox->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, Secrettarget.hitBox );
                Secrettarget.hitBox->x += camera->x;
                Secrettarget.hitBox->y += camera->y;


                ///my retarded ammo think is actually here
                ammo1.renderAmmo(player1.clip, window.gRenderer, window.getWidth() - 120, window.getHeight() - ammo1.getHeight());
				ammo2.render(window.gRenderer, window.getWidth() - ammo2.getWidth(), window.getHeight() - ammo2.getHeight());

				//Update screen
				SDL_RenderPresent( window.gRenderer );
				/******** rendering ********/

				++countedFrames;

				//If frame finished early
				int frameTicks = capTimer.getTicks();
				if( frameTicks < SCREEN_TICK_PER_FRAME )
				{
					//Wait remaining time
					SDL_Delay( SCREEN_TICK_PER_FRAME - frameTicks );
				}
			}
		}
	}

	//Free resources and close SDL
	camera = NULL;
	close();

	return 0;
}
