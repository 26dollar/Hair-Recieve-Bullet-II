#include "Texture.h"

Texture::Texture()
{
	//Initialize
	texture = NULL;
	width = 0;
	height = 0;

	animationClip = {0, 0, 0, 0};

	animationTimer1.start();
	animationTimer2.start();
	animationTimer3.start();
	ziplineTimer.start();
	ziplineTimer2.start();

    gunOffset = 0;
}

Texture::Texture(int w, int h)
{
	//Initialize
	texture = NULL;
	width = 0;
	height = 0;

	animationTimer1.start();
	animationTimer2.start();
	animationTimer3.start();
	ziplineTimer.start();
	ziplineTimer2.start();

	animationClip = {0, 0, w, h};
    gunOffset = 0;
}

Texture::~Texture()
{
	//Deallocate
	free();
}

bool Texture::loadFile( std::string path, SDL_Renderer* renderer )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Failed to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
	    //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Failed to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
		    width = loadedSurface->w;
			height = loadedSurface->h;
		}
		SDL_FreeSurface( loadedSurface );
	}
	texture = newTexture;
	newTexture = NULL;
	return texture != NULL;
}

bool Texture::loadFromRenderedText( std::string textureText, SDL_Color textColor, TTF_Font* font, SDL_Renderer* renderer )
{
    free();

	SDL_Surface* textSurface = TTF_RenderText_Solid( font, textureText.c_str(), textColor );
	if( textSurface != NULL )
    {
        texture = SDL_CreateTextureFromSurface( renderer, textSurface );
		if( texture == NULL )
		{
			printf( "Failed to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			width = textSurface->w;
			height = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	else
	{
		printf( "Failed to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}


	//Return success
	return texture != NULL;
}

void Texture::free()
{
	//Free texture if it exists
	if( texture != NULL )
	{
		SDL_DestroyTexture( texture );
		texture = NULL;
		width = 0;
		height = 0;
	}
}

void Texture::setColor( short red, short green, short blue )
{
	SDL_SetTextureColorMod( texture, red, green, blue );
}

void Texture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( texture, blending );
}

void Texture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( texture, alpha );
}

void Texture::render( SDL_Renderer* renderer, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, width, height };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( renderer, texture, clip, &renderQuad, angle, center, flip );
}

void Texture::renderAmmo(int clip, SDL_Renderer* renderer, int x, int y)
{
    animationClip.x = 53 * clip;
    SDL_Rect renderQuad = { x, y, animationClip.w, animationClip.h };
    SDL_RenderCopy( renderer, texture, &animationClip, &renderQuad);
}

void Texture::renderPlayer(int animation, SDL_Renderer* renderer, int x, int y, SDL_RendererFlip flip)
{
    switch(animation)
    {
        case 0:
            {
                animationClip.y = 0;
                animationClip.w = 65;
                animationClip.h = 136;

                if(animationTimer1.getTicks() / 150 > 3)
                {
                    animationTimer1.start();
                }

                switch(animationTimer1.getTicks() / 150)
                {
                    case 0: gunOffset = 0;
                    break;
                    case 1:
                    case 3: gunOffset = 1;
                    break;
                    case 2: gunOffset = 2;
                    break;
                }

                animationClip.x = 65 * (animationTimer1.getTicks() / 150);

                SDL_Rect renderQuad = { x, y, animationClip.w, animationClip.h };
                SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
            }
            break;
        case 2:
            {
                animationClip.y = 144;
                animationClip.w = 66;
                animationClip.h = 136;

                if(animationTimer2.getTicks() / 90 > 10)
                {
                    animationTimer2.start();
                }

                animationClip.x = 66 * (animationTimer2.getTicks() / 90);

                SDL_Rect renderQuad = { x, y, animationClip.w, animationClip.h };
                SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
            }
            break;
        case 3:
            {
                animationClip.y = 145;
                animationClip.x = 198;
                animationClip.w = 66;
                animationClip.h = 135;

                SDL_Rect renderQuad = { x, y, animationClip.w, animationClip.h };
                SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
            }
            break;
        case 4:
            {
                animationClip.y = 300;
                animationClip.x = 0;
                animationClip.w = 64;
                animationClip.h = 106;

                SDL_Rect renderQuad = { x, y, animationClip.w, animationClip.h };
                SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
            }
            break;
        case 5:
            {
                animationClip.y = 300;
                animationClip.w = 64;
                animationClip.h = 106;

                if(animationTimer3.getTicks() / 50 > 4)
                {
                    animationTimer3.start();
                }
                animationClip.x = 64 * (animationTimer3.getTicks() / 50);

                SDL_Rect renderQuad = { x, y, animationClip.w, animationClip.h };
                SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
            }
            break;
    }


}

void Texture::renderHair(SDL_Renderer* renderer, int x, int y, SDL_RendererFlip flip)
{
    if(animationTimer1.getTicks() / 50 > 10)
    {
        animationTimer1.stop();
    }

    if(animationTimer1.isStarted())
    {
        animationClip.x = 86 * (animationTimer1.getTicks() / 50);
        SDL_Rect renderQuad = { x, y, animationClip.w, animationClip.h };
        SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
    }
}

void Texture::renderZipline(SDL_Renderer* renderer, int x, int y, bool docked, bool carrying, bool powertimerstarted)
{
    if(docked)
    {
        animationClip.y = 0;
        animationClip.w = 61;
        animationClip.h = 64;

        if(ziplineTimer.getTicks() / 100 > 17)
        {
            ziplineTimer.pause();
        }
        animationClip.x = 61 * (ziplineTimer.getTicks() / 100);

        SDL_Rect renderQuad = { x, y, animationClip.w, animationClip.h };
        SDL_RenderCopy( renderer, texture, &animationClip, &renderQuad);
    }
    else if(carrying)
    {
        if(!powertimerstarted)
        {
            //ziplineTimer2.stop();
            animationClip.y = 64;
            animationClip.w = 62;
            animationClip.h = 65;
            if(ziplineTimer.getTicks() / 100 > 2)
            {
                ziplineTimer.start();
            }
            animationClip.x = 62 * (ziplineTimer.getTicks() / 100);

            SDL_Rect renderQuad = { x, y, animationClip.w, animationClip.h };
            SDL_RenderCopy( renderer, texture, &animationClip, &renderQuad);
        }
        else
        {
            animationClip.y = 64;
            animationClip.w = 62;
            animationClip.h = 65;
            if(ziplineTimer.getTicks() / 100 > 2)
            {
                ziplineTimer.start();
            }
            switch ( ziplineTimer.getTicks() / 100 )
                    {
                        case 0: animationClip.x = 124;
                            break;                                                        //+1?
                        case 1: animationClip.x = 62;          ///i have no idea where this ^ comment came from
                            break;
                        case 2: animationClip.x = 0;
                            break;
                    }
            SDL_Rect renderQuad = { x, y, animationClip.w, animationClip.h };
            SDL_RenderCopy( renderer, texture, &animationClip, &renderQuad);
        }
    }
    else
    {
        if(ziplineTimer.getTicks() / 100 > 17)
        {
            ziplineTimer.start();
        }
        SDL_Rect renderQuad = { x, y, animationClip.w, animationClip.h };
        SDL_RenderCopy( renderer, texture, &animationClip, &renderQuad);
    }
}

void Texture::renderSemi(SDL_Renderer* renderer, int x, int y, SDL_RendererFlip flip, int shootticks, int reloadticks, int offset, int direction, int animation)
{
    if(direction == 1) ///GUN POINTING UP
    {
        animationClip.w = 19;
        animationClip.h = 109;

        if(flip == SDL_FLIP_NONE)
        {
            gunOffset2 = 0;
            if(animation > 1 && animation < 4)
            {
                    gunOffset2 += 2;
            }
        }
        else
        {
            gunOffset2 = -28;

            if(animation > 3)
            {
                    gunOffset2 += 1;
            }

            if(animation > 1 && animation < 4)
            {
                    gunOffset2 -= 2;
            }
        }

        if(shootticks > 0)
        {
            animationClip.y = 65;
            animationClip.x = 19 * (shootticks / 50);
            if(shootticks / 50 > 2)
            {
                animationClip.x = 38;
            }
            SDL_Rect renderQuad = { x - gunOffset2 + 9, y + offset - 95, animationClip.w, animationClip.h };
            SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
        }
        else
        {
            animationClip.x = 0;
            animationClip.y = 65;
            SDL_Rect renderQuad = { x - gunOffset2 + 9, y + offset - 95, animationClip.w, animationClip.h };
            SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
        }
    }
    else ///GUN POINTING LEFT OR RIGHT
    {
        if(flip == SDL_FLIP_NONE)
        {
            gunOffset2 = 0;
        }
        else
        {
            gunOffset2 = 34;
        }

        if(shootticks > 0)
        {
            animationClip.w = 99;
            animationClip.h = 21;
            animationClip.y = 0;
            animationClip.x = 99 * (shootticks / 50);
            if(shootticks / 50 > 2)
            {
                animationClip.x = 198;
            }
            SDL_Rect renderQuad = { x - gunOffset2, y + offset, animationClip.w, animationClip.h };
            SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
        }
        else if(reloadticks > 0)
        {
            animationClip.w = 99;
            animationClip.h = 32;
            animationClip.y = 33;
            animationClip.x = 99 * (reloadticks / 90);
            if(reloadticks / 90 > 16)
            {
                animationClip.x = 99 * 16;
            }
            SDL_Rect renderQuad = { x - gunOffset2, y + offset + 2, animationClip.w, animationClip.h };
            SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
        }
        else
        {
            animationClip.w = 99;
            animationClip.h = 21;
            animationClip.x = 0;
            animationClip.y = 0;
            SDL_Rect renderQuad = { x - gunOffset2, y + offset, animationClip.w, animationClip.h };
            SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
        }
    }
}

void Texture::setAsRenderTarget( SDL_Renderer* renderer )
{
	//Make self render target
	SDL_SetRenderTarget( renderer, texture );
}

int Texture::getWidth()
{
	return width;
}

int Texture::getHeight()
{
	return height;
}
