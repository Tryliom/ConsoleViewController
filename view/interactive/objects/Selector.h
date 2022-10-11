#pragma once
#include <functional>

#include "../InteractiveObject.h"

namespace Console
{
	class Selector: public InteractiveObject
	{
	protected:
		std::vector<std::string> _options;
		int _selected{ 0 };
		std::function<void(int)> _setValue;
	public:
		Selector(PositionX x, PositionY y, const std::vector<std::string>& options, const std::function<void(int)>& setValue, int defaultSelected = 0, bool xCentered = false);

		void Draw(Screen& screen, bool selected) override;
		void OnKeyPress(const char key) override;
	};
}