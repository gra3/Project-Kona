#include "Button.h"
#include "Screen.h"



Button::Button()
{
}

Button::Button( SDL_Rect* rect, Screen* screen, unsigned int controlID, std::string caption ):
	caption( "Button" ),
	state( BS_IDLE ),
	idleBorderColor{ 0, 0, 0, 0 },
	hoverBorderColor{ 0, 200, 0, 0 },
	pressedBorderColor{ 200, 0, 0, 0 }

{
	Control::Init( rect, screen, controlID );

	this->caption = caption;

	MakeButtonTextures();
}

Button::~Button()
{
	DestroyButtonTextures();
}

SDL_Rect Button::CalculateInsideBorderRect()
{
	SDL_Rect insideBorder;
	auto border = static_cast< int >( GetWidth() * 0.025 );
	insideBorder = { 0, 0 , GetWidth(), GetHeight() };

	insideBorder.x += border;
	insideBorder.y += border;
	insideBorder.w -= 2 * border;
	insideBorder.h -= 2 * border;
	
	return insideBorder;
}

SDL_Rect Button::CalculateTextRegion( bool pressed )
{
	SDL_Rect textRegion = { 0, 0, GetWidth(), GetHeight() };
	auto horizontalPadding = static_cast< int >( GetWidth() * 0.1 );
	auto verticalPadding = static_cast< int >( GetHeight() * 0.1 );
	auto offset = static_cast< int >( GetWidth() * 0.025 );

	textRegion.x += horizontalPadding;
	textRegion.y += verticalPadding;
	textRegion.w -= 2 * horizontalPadding;
	textRegion.h -= 2 * verticalPadding;

	if ( pressed )
	{
		textRegion.x -= offset;
		textRegion.y += offset;
	}

	return textRegion;
}

void Button::MakeButtonTextures()
{
	if ( !buttonTextures.empty() )
		DestroyButtonTextures();

	buttonTextures.push_back( MakeButtonTexture( idleBorderColor ) );
	buttonTextures.push_back( MakeButtonTexture( hoverBorderColor ) );
	buttonTextures.push_back( MakeButtonTexture( pressedBorderColor, true ) );

	SDL_SetRenderTarget( renderer, nullptr );
}

SDL_Texture* Button::MakeButtonTexture( SDL_Color color, bool pressed /*= false */ )
{
	SDL_Texture* texture = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, dest.w, dest.h );
	SDL_SetRenderTarget( renderer, texture );
	SDL_Rect border = { 0, 0, dest.w, dest.h };
	SDL_SetRenderDrawColor( renderer, color.r, color.b, color.g, color.a );
	SDL_RenderFillRect( renderer, &border );

	SDL_Rect insideBorder = CalculateInsideBorderRect();
	SDL_SetRenderDrawColor( renderer, 255, 255, 255, 0 );
	SDL_RenderFillRect( renderer, &insideBorder );

	SDL_Rect textRegion = CalculateTextRegion( pressed );
	textRenderer->DrawInRegion( textRegion, caption );

	return texture;
}

void Button::DestroyButtonTextures()
{
	for ( auto texture : buttonTextures )
		SDL_DestroyTexture( texture );
	buttonTextures.clear();
}

void Button::SetCaption( std::string caption )
{
	this->caption = caption;
	MakeButtonTextures();
}

void Button::Draw()
{
	switch ( state )
	{
		case BS_IDLE:
			SDL_RenderCopy( renderer, buttonTextures[ BS_IDLE ], nullptr, &dest );
			break;
		case BS_HOVER:
			SDL_RenderCopy( renderer, buttonTextures[ BS_HOVER ], nullptr, &dest );
			break;
		case BS_PRESSED:
			SDL_RenderCopy( renderer, buttonTextures[ BS_PRESSED ], nullptr, &dest );
			break;
	}
}

void Button::HandleEvent( SDL_Event* event )
{
	auto oldState = state;

	if ( ContainsPoint( event->motion.x, event->motion.y ) )
	{
		if ( event->type == SDL_MOUSEBUTTONUP || event->type == SDL_MOUSEMOTION )
			state = BS_HOVER;

		if ( event->type == SDL_MOUSEBUTTONDOWN )
		{
			state = BS_PRESSED;
		}
	}
	else
		state = BS_IDLE;

	if( state != oldState )
		SendMessage( state );
}