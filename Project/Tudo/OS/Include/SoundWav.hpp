/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef SOUNDWAV_HPP_
#define SOUNDWAV_HPP_
#include "SystemTypes.hpp"
#include "Logger.hpp"
#include <soloud/soloud_wav.h>
#include <soloud/soloud_wavstream.h>

namespace Tudo
{
	template<typename T>
	class SoundWavBase
	{
	public:
		~SoundWavBase()
		{
			if(!mName.empty())
				Logger::Log("SoundWav '" + mName + "' released!");
		}

		const T& Handle() const
		{
			return mHandle;
		}

		const strg Name() const
		{
			return mName;
		}

		friend class AssetLoader;

	private:
		T		mHandle;
		strg	mName;
	};

	class SoundWav : public SoundWavBase<SoLoud::Wav> {};
	class SoundWavStream : public SoundWavBase<SoLoud::WavStream> {};
}
#endif