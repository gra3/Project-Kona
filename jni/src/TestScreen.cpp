#include "TestScreen.h"
#include "Button.h"
#include "SoundBox.h"

#include <AL\al.h>
#include <AL\alc.h>

#include <string>



TestScreen::TestScreen() :
	numberOfexecutions( 1 ),
	firstButton( nullptr ),
	secondButton( nullptr ),
	thirdButton( nullptr ),
	testBox( nullptr ),
	testBox2( nullptr )
{
}


TestScreen::~TestScreen()
{
	for ( auto c : controls )
		delete c;
}

void TestScreen::OnFirstButtonClick()
{
	numberOfexecutions += numberOfexecutions;
	//std::string str = std::to_string( numberOfexecutions );
	secondButton->SetCaption( "derp1" );
}

void TestScreen::OnFirstButtonHover()
{
}

void TestScreen::OnFirstButtonIdle()
{
}

void TestScreen::OnThirdButtonClick()
{
	numberOfexecutions = 1;
	auto secondButton = dynamic_cast< Button* >( controls[ Button_Second] );
	//std::string str = std::to_string( numberOfexecutions );
	secondButton->SetCaption( "derp2" );
}

void TestScreen::BuildControls()
{
	unsigned int controlID = 0;
	SDL_Rect buttonRect = { 50, 50, 200, 75 };
	firstButton = new Button( &buttonRect, this, controlID++, "Increment" );
	controls.push_back( firstButton );

	buttonRect.y += buttonRect.h + 10;
	//secondButton =  new Button( &buttonRect, this, controlID++, std::to_string( numberOfexecutions) );
	secondButton =  new Button( &buttonRect, this, controlID++, "derp3" );
	controls.push_back( secondButton );

	buttonRect.y += buttonRect.h + 10;
	thirdButton =  new Button( &buttonRect, this, controlID++, "Clear" );
	controls.push_back( thirdButton );

	SDL_Rect soundBoxRect = { 250, 750, 300, 300 };
	testBox = new SoundBox( &soundBoxRect, this, controlID++ );
	controls.push_back( testBox );

	soundBoxRect.x += 350;
	testBox2 = new SoundBox( &soundBoxRect, this, controlID++ );
	controls.push_back( testBox2 );

	soundBoxRect.y += 350;
	testBox3 = new SoundBox( &soundBoxRect, this, controlID++ );
	controls.push_back( testBox3 );

	soundBoxRect.x -= 350;
	testBox4 = new SoundBox( &soundBoxRect, this, controlID++ );
	controls.push_back( testBox4 );
}

void TestScreen::ProcessControlEvent( unsigned int controlID, int msg )
{
	if ( controlID == Button_First )
	{
		if ( msg == Button::BS_PRESSED )
			OnFirstButtonClick();
		else if ( msg == Button::BS_HOVER )
			OnFirstButtonHover();
		else if ( msg == Button::BS_IDLE )
			OnFirstButtonIdle();
	}
	else if ( controlID == Button_Third && msg == Button::BS_PRESSED )
		OnThirdButtonClick();
}