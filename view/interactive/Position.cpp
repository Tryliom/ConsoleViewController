#include "Position.h"
#include "../Screen.h"

Position::Position()
{
	_absolutePosition = 0;
	_relativePosition = 0.0f;
}

Position::Position(const int absolutePosition, const float relativePosition)
{
	_absolutePosition = absolutePosition;
	_relativePosition = relativePosition;
}

Position::Position(const int absolutePosition)
{
	_absolutePosition = absolutePosition;
}

Position::Position(const float relativePosition)
{
	_relativePosition = relativePosition;
}

int PositionX::GetValue() const
{
	return _absolutePosition + static_cast<int>(_relativePosition * Console::Screen::WIDTH);
}

int PositionY::GetValue() const
{
	return _absolutePosition + static_cast<int>(_relativePosition * Console::Screen::HEIGHT);
}
