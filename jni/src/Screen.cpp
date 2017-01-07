#include "Screen.h"
#include "SDL.h"


Screen::Screen()
{
}


Screen::~Screen()
{
}

void Screen::Init( SDL_Window* window )
{
	this->window = window;
}

void Screen::HandleEvents( SDL_Event* event )
{
	for ( auto c : controls )
		c->HandleEvent( event );
}

void Screen::Draw()
{
	for ( auto c : controls )
		c->Draw();
}

SDL_Window* Screen::GetWindow() const
{
	return window;
}