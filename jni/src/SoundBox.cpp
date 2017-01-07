#include "SoundBox.h"
#include <AL\al.h>
#include "SoundManager.h"
#include "Screen.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>



SoundBox::SoundBox( SDL_Rect* rect, Screen* screen, unsigned int controlID ) :
	state( SB_IDLE ),
	pitch( 1.0 ),
	yOffset( 0 ),
	maxYOffset( rect->h ),
	maxYPitchMultiplier( 0.3 ),
	xOffset( 0 ),
	maxXOffset( rect->w / 8 ),
	maxXPitchMultiplier( 0.0075 ),
	boxTexture( nullptr )
{
	SDL_Log( "geo Begin soundbox constructor\n" );
	Control::Init( rect, screen, controlID );
	SDL_Log( " geo soundbox constructor - after Init base class\n" );
	boxTexture = MakeBoxTexture();
	SDL_Log( "geo soundbox constructor - after maxboxtexture\n" );

	InitSound();
	SDL_Log( "geo soundbox constructor - after init sound\n" );
	SDL_Log( "geo End soundbox constructor\n" );
}


SoundBox::~SoundBox()
{
	SDL_DestroyTexture( boxTexture );
}

SDL_Texture* SoundBox::MakeBoxTexture()
{
	if ( boxTexture != nullptr )
		SDL_DestroyTexture( boxTexture );
	SDL_Log( "geo MakeBoxTexture - after destroy\n" );

	SDL_Color color = GenerateRandomColor();
	SDL_Log( "geo MakeBoxTexture - after color\n" );

	SDL_Texture* box = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, dest.w, dest.h );
	SDL_Log( "geo MakeBoxTexture - after create texture\n" );
	SDL_SetRenderTarget( renderer, box );
	SDL_SetRenderDrawColor( renderer, color.r, color.g, color.b, 0 );
	SDL_Rect boxROI = { 0, 0, dest.w, dest.h };
	SDL_RenderFillRect( renderer, &boxROI );

	SDL_SetRenderTarget( renderer, nullptr );
	SDL_SetRenderDrawColor( renderer, 255, 255, 255, 0 );

	return box;
}

void SoundBox::InitSound()
{
	SDL_Log( "geo Soundbox initSound\n" );
	alGenSources( ( ALuint )1, &source );
	alSourcef( source, AL_PITCH, pitch );
	alSourcef( source, AL_GAIN, 1 );
	alSource3f( source, AL_POSITION, 0, 0, 0 );
	alSource3f( source, AL_VELOCITY, 0, 0, 0 );
	alSourcei( source, AL_LOOPING, AL_FALSE );

	alGenBuffers( ( ALuint )1, &buffer );

	SoundManager::LoadWavFile( "assets/Sound/C4.wav", &buffer, &size, &freq, &format );

	alSourcei( source, AL_BUFFER, buffer );
}

void SoundBox::SetOriginPoint( int x, int y )
{
	origin.x = x;
	origin.y = y;
}

void SoundBox::SetCurrentPoint( int x, int y )
{
	current.x = x;
	current.y = y;
}

void SoundBox::SetOriginAndCurrentPoints( int x, int y )
{
	SetOriginPoint( x, y );
	SetCurrentPoint( x, y );
}

void SoundBox::CalculateYOffset()
{
	yOffset = origin.y - current.y;
	if ( yOffset > maxYOffset )
		yOffset = maxYOffset;
	if ( yOffset < -maxYOffset )
		yOffset = -maxYOffset;
}

void SoundBox::CalculateXOffset()
{
	xOffset = current.x - origin.x;
	if ( xOffset > maxXOffset )
		xOffset = maxXOffset;
	if ( xOffset < -maxXOffset )
		xOffset = -maxXOffset;
}

SDL_Color SoundBox::GenerateRandomColor()
{
	Uint8 r = rand() % 256;
	Uint8 g = rand() % 256;
	Uint8 b = rand() % 256;
	SDL_Color color = { r, b, g, 0 };

	return color;
}

void SoundBox::Draw()
{
	SDL_Rect destWithOffset = dest;
	destWithOffset.y -= yOffset;
	destWithOffset.x += xOffset;
	SDL_RenderCopy( renderer, boxTexture, nullptr, &destWithOffset );
}

void SoundBox::HandleEvent( SDL_Event* event )
{
	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode( 0, &displayMode );
	auto x = event->tfinger.x * displayMode.w;
	auto y = event->tfinger.y * displayMode.h;

	if ( ContainsPoint( x, y ) && event->type == SDL_FINGERDOWN && state == SB_IDLE )
	{
		state = SB_PRESSED;
		SetOriginAndCurrentPoints( x, y );
	}
	else if ( state == SB_PRESSED && event->type == SDL_FINGERUP )
		state = SB_IDLE;
	else if ( state == SB_PRESSED )
		SetCurrentPoint( x, y );

	if ( state == SB_PRESSED && !SoundManager::IsSourcePlaying( &source ) )
		alSourcePlay( source );
	else if ( state == SB_IDLE && SoundManager::IsSourcePlaying( &source ) )
	{
		yOffset = 0;
		xOffset = 0;
		alSourceStop( source );
		pitch = 1;
		alSourcef( source, AL_PITCH, pitch );
	}

	if ( SoundManager::IsSourcePlaying( &source ) )
	{
		CalculateYOffset();
		CalculateXOffset();

		if ( yOffset >= 0 )
			pitch = ( static_cast< double >( yOffset ) / static_cast< double >( maxYOffset ) ) * maxYPitchMultiplier + 1;
		else if ( yOffset < 0 )
			pitch = 1.0 - ( static_cast< double >( yOffset ) / static_cast< double >( maxYOffset ) ) * -maxYPitchMultiplier;

		if ( xOffset >= 0 )
			pitch = ( static_cast< double >( xOffset ) / static_cast< double >( maxXOffset ) ) * maxXPitchMultiplier + pitch;
		else if ( xOffset < 0 )
			pitch = pitch - ( static_cast< double >( xOffset ) / static_cast< double >( maxXOffset ) ) * -maxXPitchMultiplier;

		std::cout << pitch << "\n";
		alSourcef( source, AL_PITCH, pitch );
	}
}
