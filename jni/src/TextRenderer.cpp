#include "TextRenderer.h"



TextRenderer::TextRenderer()
{
}


TextRenderer::TextRenderer( SDL_Renderer* renderer, std::string fontName ):
	renderer( nullptr ),
	surface( nullptr ),
	texture( nullptr),
	font( nullptr ),
	fontName( fontName )
{
	this->renderer = renderer;
}

TextRenderer::~TextRenderer()
{
	TTF_CloseFont( font );
}

void TextRenderer::SetFont( int pointSize )
{
	if ( font != nullptr )
		TTF_CloseFont( font );

	font = TTF_OpenFont( fontName.c_str(), pointSize );
}

void TextRenderer::DrawInRegion( SDL_Rect region, std::string text, SDL_Color color )
{
	int w;
	int h;
	auto drawText = false;
	auto pointSize = static_cast< int >( region.h * 0.75 );
	SDL_Rect regionToBeDrawnTo = region;
	while ( !drawText )
	{
		SetFont( pointSize );
		surface = TTF_RenderText_Blended( font, text.c_str(), color );
		texture = SDL_CreateTextureFromSurface( renderer, surface );
		SDL_QueryTexture( texture, nullptr, nullptr, &w, &h );
		if ( w > region.w || h > region.h )
		{
			pointSize--;
			SDL_DestroyTexture( texture );
		}
		else
		{
			regionToBeDrawnTo.w = w;
			regionToBeDrawnTo.h = h;
			regionToBeDrawnTo.x += ( region.w - w ) / 2;
			regionToBeDrawnTo.y += ( region.h - h ) / 2;
			drawText = true;
		}

		SDL_FreeSurface( surface );
	}

	SDL_RenderCopy( renderer, texture, nullptr, &regionToBeDrawnTo );
	SDL_DestroyTexture( texture );
}
