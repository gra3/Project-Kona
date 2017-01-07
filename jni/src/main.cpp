// Example program:
// Using SDL2 to create an application window

#include "Button.h"
#include "SDL.h"
#include "SDL_ttf.h"

#include "Screen.h"
#include "SoundManager.h"
#include "TestScreen.h"

#include <stdio.h>
#include <time.h>

int main( int argc, char* argv[] ) {

	SDL_Window *window;                    // Declare a pointer

	SDL_Init( SDL_INIT_VIDEO );              // Initialize SDL2

	TTF_Init();

	SDL_Event event;

	SDL_Renderer* renderer;

	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode( 0, &displayMode );

	// Create an application window with the following settings:
	window = SDL_CreateWindow(
		"An SDL2 window",                  // window title
		SDL_WINDOWPOS_UNDEFINED,           // initial x position
		SDL_WINDOWPOS_UNDEFINED,           // initial y position
		displayMode.w,                               // width, in pixels
		displayMode.h,                               // height, in pixels
		SDL_WINDOW_SHOWN                  // flags - see below
	);


	// Check that the window was successfully created
	if ( window == NULL ) {
		// In the case that the window could not be made...
		printf( "Could not create window: %s\n", SDL_GetError() );
		return 1;
	}

	srand( time( nullptr ) );

	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );

	SoundManager* soundManager = new SoundManager();

	Screen* screen = new TestScreen();
	screen->Init( window );
	screen->BuildControls();

	// The window is open: could enter program loop here (see SDL_PollEvent())
	auto quit = false;
	while ( !quit )
	{
		while ( SDL_PollEvent( &event ) )
		{
			if ( event.type == SDL_QUIT )
				quit = true;

			screen->HandleEvents( &event );
		}

		SDL_RenderClear( renderer );

		screen->Draw();

		SDL_RenderPresent( renderer );
	}

	// Close and destroy the window
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );

	// Clean up
	SDL_Quit();
	return 0;
}