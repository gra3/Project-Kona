#pragma once

#include "SDL.h"
#include "TextRenderer.h"

class Screen;

class Control
{
protected:
	unsigned int controlID;
	SDL_Rect dest;
	SDL_Renderer* renderer;
	TextRenderer* textRenderer;
	Screen* screen;

public:
	virtual ~Control();

	void Init( SDL_Rect* dest, Screen* screen, unsigned int ID );
	virtual void Draw() = 0;
	virtual void HandleEvent( SDL_Event* event ) = 0;
	int GetWidth() const;
	int GetHeight() const;
	bool ContainsPoint( int x, int y ) const;
	void SendMessage( int message );
};

