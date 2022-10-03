#pragma once

namespace Console
{
	enum class Foreground
	{
		NONE,
		BLACK = 30,
		RED = 31,
		GREEN = 32,
		YELLOW = 33,
		BLUE = 34,
		MAGENTA = 35,
		CYAN = 36,
		WHITE = 37,
		ULTRA_WHITE = 97,
		DARK_GRAY = 90,
		LIGHT_MAGENTA = 95,
	};

	enum class Background
	{
		NONE,
		BLACK = 40,
		RED = 41,
		GREEN = 42,
		YELLOW = 43,
		BLUE = 44,
		MAGENTA = 45,
		CYAN = 46,
		WHITE = 47,
		ULTRA_WHITE = 107,
		DARK_GRAY = 100,
		LIGHT_MAGENTA = 105,
	};

	struct Key
	{
		inline static int Up = 72,
			Down = 80,
			Left = 75,
			Right = 77,
			Enter = 13,
			Escape = 27,
			Space = 32,
			Backspace = 8,
			Tab = 9,
			Delete = 83,
			Home = 71;
	};
}