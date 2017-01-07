#pragma once

#include "Screen.h"

#include <AL\alc.h>
#include <AL\al.h>

class Button;
class SoundBox;

class TestScreen :
	public Screen
{
private:
	enum Controls
	{
		Button_First,
		Button_Second,
		Button_Third,
		SoundBox_TestBox,
		SoundBox_TextBox2,
		SoundBox_TextBox3,
		SoundBox_TextBox4
	};
	int numberOfexecutions;

	Button* firstButton;
	Button* secondButton;
	Button* thirdButton;

	SoundBox* testBox;
	SoundBox* testBox2;
	SoundBox* testBox3;
	SoundBox* testBox4;

public:
	TestScreen();
	~TestScreen();

	void OnFirstButtonClick();
	void OnFirstButtonHover();
	void OnFirstButtonIdle();
	void OnThirdButtonClick();

	//Screen contract
	virtual void BuildControls();
	virtual void ProcessControlEvent( unsigned int controlID, int msg );
};

