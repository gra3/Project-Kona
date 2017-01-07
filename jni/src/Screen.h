#pragma once

#include "Control.h"

#include <vector>
class Screen
{
protected:
	SDL_Window* window;
	std::vector< Control* > controls;
public:
	Screen();
	~Screen();
	
	void Init( SDL_Window* window );
	virtual void BuildControls() = 0;
	virtual void HandleEvents( SDL_Event* event );
	virtual void ProcessControlEvent( unsigned int controlID, int msg ) = 0;
	void Draw();
	SDL_Window* GetWindow() const;
};

