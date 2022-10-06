#include "AudioManager.h"
#pragma comment(lib, "Winmm.lib")
#include <windows.h>

namespace Console::AudioManager
{
	void Play(const std::string& path)
	{
		sndPlaySoundA(path.c_str(), SND_FILENAME | SND_ASYNC);
	}

	void Stop()
	{
		PlaySound(nullptr, nullptr, SND_ASYNC);
	}
}
