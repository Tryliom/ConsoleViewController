#include "AudioManager.h"
#pragma comment(lib, "Winmm.lib")
#include <windows.h>

namespace Console::AudioManager
{
	void Play(const std::string& path, const bool repeat)
	{
		PlaySoundA(path.c_str(), nullptr, SND_FILENAME | SND_ASYNC | (repeat ? SND_LOOP : 0));
	}

	void Stop()
	{
		PlaySound(nullptr, nullptr, SND_ASYNC);
	}
}
