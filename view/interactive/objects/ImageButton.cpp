#include "ImageButton.h"

namespace Console
{
	ImageButton::ImageButton(Position x, Position y, const Image& image, const std::function<void()>& onClick, const bool xCentered,
							const bool yCentered, const Background background, const Foreground foreground) :
		InteractiveObject(x, y, xCentered)
	{
		_image = image;
		_onClick = onClick;
		_yCentered = yCentered;
		_background = background;
		_foreground = foreground;
	}

	void ImageButton::Draw(Controller* controller, Screen& screen, bool selected)
	{
		auto background = Background::NONE;
		auto foreground = Foreground::NONE;
		int y = _y.GetValue(false);
		int x = _x.GetValue(true);

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

	void ImageButton::OnKeyPress(Controller* controller, const char key)
	{
		if (key == Key::Enter)
		{
			_onClick();
		}
	}
}
