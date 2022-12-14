#include "screen.h"

#include <iostream>
#include <ranges>
#include <thread>
#include <windows.h>

#include "interactive/Position.h"

constexpr COLORREF DEFAULT_BACKGROUND_COLOR = RGB(12, 12, 12);

namespace Console
{
	void setCursorVisibility(const bool visibility)
	{
		const HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_CURSOR_INFO cursorInfo;
		GetConsoleCursorInfo(output, &cursorInfo);
		cursorInfo.bVisible = visibility;
		SetConsoleCursorInfo(output, &cursorInfo);
	}

	void Screen::setPos(const int x, const int y)
	{
		// Set the position of the cursor
		COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
		const HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleCursorPosition(output, pos);
	}

	Screen::Screen()
	{
		this->_screen = {};
		this->_cache = {};
		this->_pixelColorsMap = {};
		this->_pixelColorsMapCache = {};
	}

	void Screen::Reset()
	{
		// Set the height and width according to the console window
		const HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
		const HWND console = GetConsoleWindow();
		RECT r;
		GetWindowRect(console, &r);
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(output, &csbi);

		// Redefine the height and width of the screen
		int oldWidth = WIDTH_PIXEL;
		int oldHeight = HEIGHT_PIXEL;
		HEIGHT = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
		WIDTH = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		HEIGHT_PIXEL = r.bottom - r.top - 40;
		WIDTH_PIXEL = r.right - r.left - 16 * 2;
		PIXEL_RATIO_Y = HEIGHT_PIXEL / HEIGHT;
		PIXEL_RATIO_X = WIDTH_PIXEL / WIDTH;

		this->_cache = this->_screen;
		this->_screen = {};
		this->_pixelColorsMapCache = this->_pixelColorsMap;
		this->_pixelColorsMap = {};

		if (oldWidth != WIDTH_PIXEL || oldHeight != HEIGHT_PIXEL)
		{
			_cache.clear();
			_frameCountdown = 5;
		}

		if (_frameCountdown > 0)
		{
			_pixelColorsMapCache.clear();
			_frameCountdown--;
		}

		if (!PIXEL_CACHE)
		{
			_pixelColorsMapCache.clear();
		}

		for (auto i = _pixelColorsMapCache.begin(); i != _pixelColorsMapCache.end();)
		{
			if (i->second.Color == DEFAULT_BACKGROUND_COLOR)
			{
				i = _pixelColorsMapCache.erase(i);
			}
			else
			{
				++i;
			}
		}

		// Hide the cursor
		setCursorVisibility(false);

		// Fill the screen with spaces and # if BORDER is true for borders
		for (int h = 0; h < HEIGHT; h++)
		{
			std::vector<std::string> row;
			for (int w = 0; w < WIDTH; w++)
			{
				if (BORDER)
				{
					if (h == 0 || h == HEIGHT - 1)
					{
						row.emplace_back(BORDER_ROW);
					}
					else if (w == 0 || w == WIDTH - 1)
					{
						row.emplace_back(BORDER_COLUMN);
					}
					else
					{
						row.emplace_back(" ");
					}
				}
				else
				{
					row.emplace_back(" ");
				}
			}

			this->_screen.emplace_back(row);
		}

		// Reset cursor position
		_cursorX = -1;
		_cursorY = -1;
	}

	void Screen::Render()
	{
		const HWND console = GetConsoleWindow();
		const HDC hdc = GetDC(console);

		// Display every lines of the screen
		for (int h = 0; h < HEIGHT; h++)
		{
			bool hasDraw = false;
			int minW = 0;
			int maxW = 0;

			for (int w = 0; w < WIDTH; w++)
			{
				// If the cache has a different size than the screen, display the whole screen, otherwise only the differences between the cache and the screen
				if (static_cast<int>(this->_cache.size()) != HEIGHT || static_cast<int>(this->_cache[h].size()) != WIDTH || this->_cache[h][w] != _screen[h][w])
				{
					this->setPos(w, h);
					std::cout << _screen[h][w];
					if (!hasDraw)
					{
						minW = w;
						hasDraw = true;
					}
					else
					{
						maxW = w;
					}
				}
			}

			// If the line has been drawn, remove the line from the pixel cache
			if (hasDraw)
			{
				for (auto i = _pixelColorsMapCache.begin(); i != _pixelColorsMapCache.end();)
				{
					if ((i->first.Y >= PositionY(h - 2).GetValue(true) && i->first.X >= PositionX(minW - 2).GetValue(true))
						|| (i->first.Y <= PositionY(h + 2).GetValue(true) && i->first.X <= PositionX(maxW + 2).GetValue(true)))
					{
						i->second.Color = DEFAULT_BACKGROUND_COLOR;
					}

					++i;
				}
			}
		}

		// Doesn't display pixels that are already displayed on the screen
		for (const auto& key : _pixelColorsMapCache | std::views::keys)
		{
			if (_pixelColorsMap.contains(key))
			{
				if (_pixelColorsMap[key].Color == _pixelColorsMapCache[key].Color)
				{
					_pixelColorsMap[key].Display = false;
				}

				continue;
			}

			_pixelColorsMap[key] = PixelColor(key.X, key.Y, DEFAULT_BACKGROUND_COLOR);
		}

		// Display all pixels that are not already displayed on the screen
		for (auto& value : _pixelColorsMap | std::views::values)
		{
			if (value.Display)
			{
				SetPixel(hdc, value.X, value.Y, value.Color);
			}
			else
			{
				value.Display = true;
			}
		}

		if (_cursorX != -1 && _cursorY != -1)
		{
			// Show the cursor
			setCursorVisibility(true);
			this->setPos(_cursorX, _cursorY);
		}

		ReleaseDC(console, hdc);
	}

	void Screen::Draw(Text text)
	{
		if (text.XCentered)
		{
			text.X -= static_cast<int>(text.Str.length()) / 2;
		}

		// If the text is out of the screen, don't draw it
		if (HEIGHT <= text.Y || WIDTH <= text.X)
		{
			return;
		}

		// Colorize the Str if a background or foreground color is specified
		std::string preColor, postColor;

		if (text.Background != Background::NONE || text.Foreground != Foreground::NONE)
		{
			std::string colors;
			preColor = "\033[";
			postColor = "\033[0m";

			if (text.Background != Background::NONE)
			{
				colors += std::to_string(static_cast<int>(text.Background));
			}
			if (text.Foreground != Foreground::NONE)
			{
				if (!colors.empty())
				{
					colors += ";";
				}
				colors += std::to_string(static_cast<int>(text.Foreground));
			}

			preColor += colors + "m";
		}

		for (int i = 0; i < static_cast<int>(text.Str.size()); i++)
		{
			// Break if the text is out of the screen
			if (text.X + i >= WIDTH)
			{
				break;
			}
			// Draw the character at the specified position with the specified color if any
			this->_screen[text.Y][text.X + i] = preColor + text.Str[i] + postColor;
		}
	}

	void Screen::Draw(const PixelColor pixelColor)
	{
		if (pixelColor.X >= 0 && pixelColor.X < WIDTH_PIXEL && pixelColor.Y >= 0 && pixelColor.Y < HEIGHT_PIXEL)
		{
			// Search if there is already a pixel at the same position
			const auto pixel = Pixel(pixelColor.X, pixelColor.Y);

			if (_pixelColorsMap.contains(pixel))
			{
				_pixelColorsMap[pixel].Color = pixelColor.Color;
			}
			else
			{
				_pixelColorsMap[pixel] = pixelColor;
			}
		}
	}

	void Screen::Draw(const Image& image, const int x, const int y, const bool centeredX, const bool centeredY)
	{
		int finalX = x;
		int finalY = y;

		if (centeredX)
		{
			finalX -= image.GetWidth() / 2;
		}
		if (centeredY)
		{
			finalY -= image.GetHeight() / 2;
		}

		for (const auto& line : image.GetImage())
		{
			Draw(Text(line, finalX, finalY));
			finalY++;
		}
	}

	void Screen::DrawRect(const int x, const int y, const int width, const int height, const COLORREF color, const bool center)
	{
		int startX = x;
		int startY = y;

		if (center)
		{
			startX -= width / 2;
			startY -= height / 2;
		}

		for (int h = 0; h < height; h++)
		{
			for (int w = 0; w < width; w++)
			{
				Draw(PixelColor(startX + w, startY + h, color));
			}
		}
	}

	void Screen::DrawCircle(const int x, const int y, const int radius, const COLORREF color, const bool center)
	{
		int startX = x;
		int startY = y;

		if (center)
		{
			startX -= radius;
			startY -= radius;
		}

		for (int h = 0; h < radius * 2; h++)
		{
			for (int w = 0; w < radius * 2; w++)
			{
				if (sqrt(pow(h - radius, 2) + pow(w - radius, 2)) <= radius)
				{
					Draw(PixelColor(startX + w, startY + h, color));
				}
			}
		}
	}

	void Screen::DrawLine(int x1, int y1, const int x2, const int y2, const int width, const COLORREF color)
	{
		const int dx = abs(x2 - x1);
		const int sx = x1 < x2 ? 1 : -1;
		const int dy = -abs(y2 - y1);
		const int sy = y1 < y2 ? 1 : -1;
		int err = dx + dy;

		while (true)
		{
			DrawRect(x1, y1, width, width, color, true);

			if (x1 == x2 && y1 == y2)
			{
				break;
			}

			const int e2 = 2 * err;

			if (e2 >= dy)
			{
				err += dy;
				x1 += sx;
			}

			if (e2 <= dx)
			{
				err += dx;
				y1 += sy;
			}
		}
	}

	void Screen::SetWindowSize(const int width, const int height)
	{
		const HWND console = GetConsoleWindow();
		RECT r;
		GetWindowRect(console, &r);
		MoveWindow(console, r.left, r.top, width, height, TRUE);
	}

	void Screen::SetTitle(const std::string& title)
	{
		SetConsoleTitleA(title.c_str());
	}

	void Screen::CenterWindow()
	{
		const HWND console = GetConsoleWindow();
		RECT r;
		GetWindowRect(console, &r);
		const int width = r.right - r.left;
		const int height = r.bottom - r.top;
		const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		const int screenHeight = GetSystemMetrics(SM_CYSCREEN);
		MoveWindow(console, (screenWidth - width) / 2, (screenHeight - height) / 2, width, height, TRUE);
	}

	void Screen::EnableVirtualTerminalProcessing()
	{
		const HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		DWORD dwMode = 0;
		GetConsoleMode(handle, &dwMode);
		if (!(dwMode & ENABLE_VIRTUAL_TERMINAL_PROCESSING))
		{
			dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
			SetConsoleMode(handle, dwMode);
		}
	}
}
