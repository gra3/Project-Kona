#pragma once

#include "Control.h"
#include "SDL.h"

#include <iostream>
#include <vector>

class Button:
	public Control
{
public:
	enum ButtonState
	{
		BS_IDLE,
		BS_HOVER,
		BS_PRESSED
	};

private:
	std::string caption;
	ButtonState state;

	SDL_Color idleBorderColor;
	SDL_Color hoverBorderColor;
	SDL_Color pressedBorderColor;

	std::vector< SDL_Texture* > buttonTextures;

public:
	Button();
	Button( SDL_Rect* rect, Screen* screen, unsigned int controlID, std::string caption );
	~Button();

	SDL_Rect CalculateInsideBorderRect();
	SDL_Rect CalculateTextRegion( bool pressed );

	void MakeButtonTextures();
	SDL_Texture* MakeButtonTexture( SDL_Color c, bool pressed = false );
	void DestroyButtonTextures();

	void SetCaption( std::string caption );

	// Control contract
	virtual void Draw();
	virtual void HandleEvent( SDL_Event* event );
};

