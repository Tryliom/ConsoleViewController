#pragma once
#include <string>

namespace Console::AudioManager
{
	/**
	 * \brief Plays the audio file at the given path (must be a .wav file)
	 * \param path The path to the audio file
	 */
	void Play(const std::string& path);
	void Stop();
}

