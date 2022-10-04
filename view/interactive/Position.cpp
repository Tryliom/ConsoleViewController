#include "Position.h"
#include "../Screen.h"

void Position::check()
{
	if (_absolutePosition < 0)
	{
		_absolutePosition = 0;
	}

	if (_relativePosition < 0.0f)
	{
		_relativePosition = 0.0f;
	}
}

Position::Position()
{
	_absolutePosition = 0;
	_relativePosition = 0.0f;
}

Position::Position(const int absolutePosition, const float relativePosition)
{
	_absolutePosition = absolutePosition;
	_relativePosition = relativePosition;
	check();
}

Position::Position(const int absolutePosition)
{
	_absolutePosition = absolutePosition;
	check();
}

Position::Position(const float relativePosition)
{
	_relativePosition = relativePosition;
	check();
}

int Position::GetValue(const bool isX) const
{
	return _absolutePosition + static_cast<int>(_relativePosition * (isX ? Console::Screen::WIDTH : Console::Screen::HEIGHT));
}
