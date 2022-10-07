#pragma once
#include "Image.h"
#include "../Constants.h"

#include <string>
#include <unordered_map>
#include <vector>
#include <windows.h>

namespace Console
{
	// If true, a border will be drawn around the screen
	bool constexpr BORDER = true;
	// The character used to draw the columns for the border
	std::string const BORDER_COLUMN = "|";
	// The character used to draw the rows for the border
	std::string const BORDER_ROW = "=";

	struct Text
	{
		std::string Str{};
		int X{ 0 };
		int Y{ 0 };
		bool XCentered{ false };
		Background Background{ Background::NONE };
		Foreground Foreground{ Foreground::NONE };
	};

	struct Pixel
	{
		int X;
		int Y;

		bool operator==(const Pixel& other) const
		{
			return this->X == other.X && this->Y == other.Y;
		}

		bool operator()(const Pixel& lhs, const Pixel& rhs) const
		{
			return lhs.X < rhs.X || (lhs.X == rhs.X && lhs.Y < rhs.Y);
		}

		std::size_t operator()(const Pixel& pixel) const
		{
			return std::hash<int>()(pixel.X) ^ std::hash<int>()(pixel.Y);
		}

		bool operator<(const Pixel& other) const
		{
			return this->X < other.X || (this->X == other.X && this->Y < other.Y);
		}
	};

	struct PixelColor
	{
		int X;
		int Y;
		COLORREF Color;
		bool Display{true};

		PixelColor() = default;
		PixelColor(const int x, const int y, const COLORREF color)
		{
			X = x;
			Y = y;
			Color = color;
		}
	};

	class Screen
	{
	private:
		std::vector<std::vector<std::string>> _screen;
		// The cache of the previous screen
		std::vector<std::vector<std::string>> _cache;
		std::map<Pixel, PixelColor> _pixelColorsMap;
		std::map<Pixel, PixelColor> _pixelColorsMapCache;
		// The cursor position on the screen if it is displayed
		int _cursorX;
		int _cursorY;

		static void setPos(int x, int y);
	public:
		Screen();

		inline static int HEIGHT = 0;
		inline static int WIDTH = 0;
		inline static int HEIGHT_PIXEL = 0;
		inline static int WIDTH_PIXEL = 0;
		// The ratio you need to multiply the width and height of the screen to get the width and height of the pixel screen
		inline static int PIXEL_RATIO_X = 1;
		inline static int PIXEL_RATIO_Y = 1;

		/**
		 * \brief Clear the screen and prepare it for a new display
		 */
		void Reset();
		/**
		 * \brief Render the screen
		 */
		void Render();
		/**
		 * \brief Draw a text on the screen
		 * \param text The text to draw
		 */
		void Draw(Text text);
		/**
		 * \brief Draw a pixel of a color on the screen
		 * \param pixelColor The pixel to draw
		 */
		void Draw(PixelColor pixelColor);
		/**
		 * \brief Draw an image on the screen
		 * \param image The image to draw
		 * \param x The x position of the image
		 * \param y The y position of the image
		 * \param centeredX If true, the image will be centered on the x axis
		 * \param centeredY If true, the image will be centered on the y axis
		 */
		void Draw(const Image& image, int x, int y, bool centeredX = false, bool centeredY = false);
		/**
		 * \brief Draw a rectangle of a color on the screen
		 * \param x The x position of the rectangle
		 * \param y The y position of the rectangle
		 * \param width The width of the rectangle
		 * \param height The height of the rectangle
		 * \param color The color of the rectangle
		 * \param center If true, the rectangle will be centered on the specified position
		 */
		void DrawRect(int x, int y, int width, int height, COLORREF color, bool center = false);
		/**
		 * \brief Draw a circle of a color on the screen
		 * \param x The x position of the circle
		 * \param y The y position of the circle
		 * \param radius The radius of the circle
		 * \param color The color of the circle
		 * \param center If true, the circle will be centered on the specified position
		 */
		void DrawCircle(int x, int y, int radius, COLORREF color, bool center = false);
		/**
		 * \brief Draw a line of a color on the screen
		 * \param x1 The x position of the first point
		 * \param y1 The y position of the first point
		 * \param x2 The x position of the second point
		 * \param y2 The y position of the second point
		 * \param width The width of the line
		 * \param color The color of the line
		 */
		void DrawLine(int x1, int y1, int x2, int y2, int width, COLORREF color);

		/**
		 * \brief Set windows size
		 * \param width The width of the window as pixel
		 * \param height The height of the window as pixel
		 */
		static void SetWindowSize(int width, int height);
		/**
		 * \brief Rename the window
		 * \param title The new title of the window
		 */
		static void SetTitle(const std::string& title);
		/**
		 * \brief Center the window on the screen
		 */
		static void CenterWindow();
		/**
		 * \brief You need to enable this if you want to use colors
		 */
		static void EnableVirtualTerminalProcessing();
		/**
		 * \brief Set the cursor position for this frame
		 * \param x The x position of the cursor
		 * \param y The y position of the cursor
		 */
		void SetCursor(const int x, const int y) { this->_cursorX = x; this->_cursorY = y; }
	};
}

