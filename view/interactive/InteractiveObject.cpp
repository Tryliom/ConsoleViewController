#include "InteractiveObject.h"

namespace Console {
	InteractiveObject::InteractiveObject(Position x, Position y, const bool xCentered)
	{
		_x = x;
		_y = y;
		_xCentered = xCentered;
	}
}
