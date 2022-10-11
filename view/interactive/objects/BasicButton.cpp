#include "BasicButton.h"

namespace Console
{
	BasicButton::BasicButton(const std::string& str, PositionX x, PositionY y, const std::function<void()>& onClick, const bool xCentered, const bool yCentered) :
		InteractiveObject(x, y, xCentered)
	{
		_str = str;
		_yCentered = yCentered;
		_onClick = onClick;
	}

	void BasicButton::Draw(Screen& screen, const bool selected)
	{
		auto background = _background;
		auto foreground = _foreground;
		int y = _y.GetValue();
		int x = _x.GetValue();

		if (selected)
		{
			background = _selectedBackground;
			foreground = _selectedForeground;
		}

		if (_yCentered)
		{
			y -= 1;
		}

		// Draw a border around the button
		std::string border;
		for (int i = 0; i < static_cast<int>(_str.length()) + 2; i++)
		{
			border += _rowBorder;
		}

		std::string text = _columnBorder + _str + _columnBorder;

		screen.Draw(Text{ .Str = border, .X = x, .Y = y, .XCentered = _xCentered, .Background = background, .Foreground = foreground });
		screen.Draw(Text{ .Str = text, .X = x, .Y = y + 1, .XCentered = _xCentered, .Background = background, .Foreground = foreground });
		screen.Draw(Text{ .Str = border, .X = x, .Y = y + 2, .XCentered = _xCentered, .Background = background, .Foreground = foreground });
	}

	void BasicButton::OnKeyPress(const char key)
	{
		if (key == Key::Enter)
		{
			_onClick();
		}
	}
}