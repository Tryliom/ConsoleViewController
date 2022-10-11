#pragma once
#include <functional>

#include "Position.h"
#include "../Screen.h"
#include "../../Constants.h"
#include "../../controller/Controller.h"

namespace Console
{
	class Controller;

	class InteractiveObject
	{
	protected:
		PositionX _x;
		PositionY _y;
		bool _xCentered{ false };
		Background _background{ Background::NONE };
		Foreground _foreground{ Foreground::NONE };
		Background _selectedBackground{ Background::CYAN };
		Foreground _selectedForeground{ Foreground::BLACK };

	public:
		InteractiveObject(PositionX x, PositionY y, bool xCentered = false);

		virtual void Draw(Screen& screen, bool selected) = 0;
		virtual void OnKeyPress(const char key) = 0;
	};


}


