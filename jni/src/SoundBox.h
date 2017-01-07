#pragma once
#include "Control.h"
#include "SDL.h"
#include <AL\al.h>

class SoundBox :
	public Control
{
public:
	enum SoundBoxState
	{
		SB_IDLE,
		SB_PRESSED
	};

private:
	SoundBoxState state;
	SDL_Texture* boxTexture;
	SDL_Color boxColor;
	
	SDL_Point origin;
	SDL_Point current;
	int yOffset;
	int maxYOffset;
	double maxYPitchMultiplier;
	int xOffset;
	int maxXOffset;
	double maxXPitchMultiplier;

	ALuint source;
	ALuint buffer;
	ALsizei size;
	ALsizei freq;
	ALenum format;
	ALfloat pitch;

public:
	SoundBox( SDL_Rect* rect, Screen* screen, unsigned int controlID );
	~SoundBox();

	SDL_Texture* MakeBoxTexture();
	void InitSound();

	void SetOriginPoint( int x, int y );
	void SetCurrentPoint( int x, int y );
	void SetOriginAndCurrentPoints( int x, int y );

	void CalculateYOffset();
	void CalculateXOffset();

	SDL_Color GenerateRandomColor();

	//Control contract
	virtual void Draw();
	virtual void HandleEvent( SDL_Event* event );
};

