#include "ImageButton.h"

namespace Console
{
	ImageButton::ImageButton(PositionX x, PositionY y, const Image& image, const std::function<void()>& onClick, const bool xCentered,
							const bool yCentered, const Background background, const Foreground foreground) :
		InteractiveObject(x, y, xCentered)
	{
		_image = image;
		_onClick = onClick;
		_yCentered = yCentered;
		_background = background;
		_foreground = foreground;
	}

	void ImageButton::Draw(Screen& screen, bool selected)
	{
		auto background = Background::NONE;
		auto foreground = Foreground::NONE;
		int y = _y.GetValue();
		int x = _x.GetValue();

		if (_yCentered)
		{
			y -= _image.GetHeight() / 2;
		}

		if (selected)
		{
			background = _background;
			foreground = _foreground;
		}

		for (const std::string& row : _image.GetImage())
		{
			screen.Draw(Text{ .Str = row, .X = x, .Y = y, .XCentered = _xCentered, .Background = background, .Foreground = foreground });
		}
	}

	void ImageButton::OnKeyPress(const char key)
	{
		if (key == Key::Enter)
		{
			_onClick();
		}
	}
}
