#pragma once
#include <string>

namespace Console::AudioManager
{
	/**
	 * \brief Plays the audio file at the given path (must be a .wav file)
	 * \param path The path to the audio file
	 * \param repeat Whether or not the audio should repeat
	 */
	void Play(const std::string& path, bool repeat);
	void Stop();
}

