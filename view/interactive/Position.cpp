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

int PositionX::GetValue(const bool usePixel)
{
	int value = _absolutePosition + static_cast<int>(_relativePosition * static_cast<float>(Console::Screen::WIDTH));

	if (usePixel)
	{
		value *= Console::Screen::PIXEL_RATIO_X;
	}

	return value;
}

int PositionY::GetValue(const bool usePixel)
{
	int value = _absolutePosition + static_cast<int>(_relativePosition * static_cast<float>(Console::Screen::HEIGHT));

	if (usePixel)
	{
		value *= Console::Screen::PIXEL_RATIO_Y;
	}

	return value;
}
