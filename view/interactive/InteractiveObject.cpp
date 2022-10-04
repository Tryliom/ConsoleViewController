#include "InteractiveObject.h"

namespace Console {
	InteractiveObject::InteractiveObject(const PositionX x, const PositionY y, const bool xCentered)
	{
		_x = x;
		_y = y;
		_xCentered = xCentered;
	}
}
