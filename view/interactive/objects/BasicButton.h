#pragma once
#include "../InteractiveObject.h"

#include <functional>

namespace Console
{
	class BasicButton : public InteractiveObject
	{
	protected:
		std::string _str{};
		bool _yCentered{ false };
		std::function<void()> _onClick{};
		std::string _rowBorder{ "-" };
		std::string _columnBorder{ "|" };

	public:
		BasicButton(const std::string& str, PositionX x, PositionY y, const std::function<void()>& onClick, bool xCentered = false, bool yCentered = false);

		void SetText(const std::string& str) { _str = str; }

		void Draw(Screen& screen, bool selected) override;
		void OnKeyPress(char key) override;
	};
}

