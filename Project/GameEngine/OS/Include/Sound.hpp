/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef SOUND_HPP_
#define SOUND_HPP_
#include "SystemTypes.hpp"
#include <soloud/soloud.h>
#include <soloud/soloud_wav.h>
#include <soloud/soloud_wavstream.h>
#include <vector>

namespace GameEngine
{
	typedef SoLoud::handle		SoundHandle;
	typedef SoLoud::AudioSource	SoundSrc;
	typedef SoLoud::Wav			SoundWav;
	typedef SoLoud::WavStream	SoundWavStream;
	typedef SoLoud::Soloud		SoundCore;
	typedef SoLoud::result		SoundResult;

	/// <summary>
	/// Manages audio
	/// </summary>
	class Sound
	{
	public:
		static void LoadFromFile(SoundWav& obj, strgv filepath);
		static void StreamFromFile(SoundWavStream& obj, strgv filepath);
		static void LoadFromMemory(SoundWav& obj, std::vector<uint8>& data);

		void Initialize();
		~Sound();

		SoundCore* Core();

	private:
		SoundCore* mCore;
	};
}
#endif