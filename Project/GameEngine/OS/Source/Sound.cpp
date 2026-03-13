/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "Sound.hpp"
#include "BigError.hpp"

using namespace GameEngine;

void Sound::LoadFromFile(SoundWav& obj, strgv filepath)
{
	if (obj.load(filepath.data()) != SoLoud::SO_NO_ERROR)
	{
		const strg errmsg = "Failed loading sound file: " + strg(filepath);
		throw BigError(errmsg);
	}
}

void Sound::StreamFromFile(SoundWavStream& obj, strgv filepath)
{
	if (obj.load(filepath.data()) != SoLoud::SO_NO_ERROR)
	{
		const strg errmsg = "Failed streaming sound file: " + strg(filepath);
		throw BigError(errmsg);
	}
}

void Sound::LoadFromMemory(SoundWav& obj, std::vector<uint8>& data)
{
	if (obj.loadMem(data.data(), static_cast<uint>(data.size()), false, false) != SoLoud::SO_NO_ERROR)
		throw BigError("Failed loading sound file from memory!");
}

void Sound::Initialize()
{
	mCore = new SoundCore();
	SoundResult err = mCore->init();
	if (err != SoLoud::SO_NO_ERROR)
		throw BigError("Failed initializing sound engine!");
	mCore->set3dListenerUp(0.0f, 1.0f, 0.0f);
}

Sound::~Sound()
{
	mCore->deinit();
	delete mCore;
}

SoundCore* Sound::Core()
{
	return mCore;
}
