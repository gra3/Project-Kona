#include "Control.h"
#include "Screen.h"


Control::~Control()
{
	delete textRenderer;
}

void Control::Init( SDL_Rect* dest, Screen* screen, unsigned int ID )
{
	controlID = ID;
	this->dest = *dest;
	this->screen = screen;
	this->renderer = SDL_GetRenderer( screen->GetWindow() );
	textRenderer = new TextRenderer( renderer, "arial.ttf" );
}

int Control::GetWidth() const
{
	return dest.w;
}

int Control::GetHeight() const
{
	return dest.h;
}

bool Control::ContainsPoint( int x, int y ) const
{
	auto withInX = x >= dest.x && x <= ( dest.x + dest.w );
	auto withInY = y >= dest.y && y <= ( dest.y + dest.h );

	return withInX && withInY;
}

void Control::SendMessage( int message )
{
	screen->ProcessControlEvent( controlID, message );
}