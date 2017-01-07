#pragma once

#include "SDL.h"
#include "SDL_ttf.h"

#include <iostream>

class TextRenderer
{
private:
	SDL_Renderer* renderer;
	SDL_Surface* surface;
	SDL_Texture* texture;

	TTF_Font* font;
	std::string fontName;

public:
	TextRenderer();
	TextRenderer( SDL_Renderer* renderer, std::string fontName );
	~TextRenderer();

	void SetFont( int pointSize );
	void DrawInRegion( SDL_Rect region, std::string text, SDL_Color color = { 0, 0, 0, 0 } );
};

