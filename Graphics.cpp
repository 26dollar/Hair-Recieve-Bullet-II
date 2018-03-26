#include "Graphics.h"

Graphics::Graphics()
{
    SCREEN_WIDTH = 960;
    SCREEN_HEIGHT = 540;
    SDL_Window* gWindow = NULL;
    SDL_Renderer* gRenderer = NULL;
    success = true;
}

Graphics::~Graphics()
{
    SCREEN_HEIGHT = 0;
    SCREEN_WIDTH = 0;
}

Graphics::getWidth()
{
    return SCREEN_WIDTH;
}

Graphics::getHeight()
{
    return SCREEN_HEIGHT;
}

bool Graphics::Initialize()
{
    //Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0 )
	{
		printf( "Whore ass. SDL could not initialize \n SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//set texture filtering
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Linear texture filtering not enabled" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window creation failed \n SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer creation failed \n SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image failed to initialize \n SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}

				 //Initialize SDL_ttf
                /*
                (if( TTF_Init() == -1 )
                {
                        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                        success = false;
                }
                */

				//Initialize SDL_mixer
				if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
                {
                    printf("SDL_mixer failed to initialize \n SDL Error: %s/n", Mix_GetError());
                    success = false;
                }
			}
		}
	}
	return success;
}
