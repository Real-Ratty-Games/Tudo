/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "Sound.hpp"
#include "Logger.hpp"

using namespace Tudo;

SoundManager::SoundManager()
{
	mCore = new SoundCore();
	SoundResult err = mCore->init();
	if (err != SoLoud::SO_NO_ERROR)
		Logger::Log("SoundManager::SoundManager", "Failed initializing sound engine!", ELogType::LERROR);
	mCore->set3dListenerUp(0.0f, 1.0f, 0.0f);

	Logger::Log("SoundManager successfully initialized!");
}

SoundManager::~SoundManager()
{
	mCore->deinit();
	delete mCore;
	Logger::Log("SoundManager released!");
}

SoundCore* SoundManager::Core()
{
	return mCore;
}
