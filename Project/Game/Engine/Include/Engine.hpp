/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef ENGINE_HPP_
#define ENGINE_HPP_
#include <SystemTypes.hpp>

#include <Program.hpp>
#include <Clock.hpp>

#include <DrawData.hpp>
#include <GraphicsDevice.hpp>
#include <DrawPipeline.hpp>

#include <Shader.hpp>
#include <Texture.hpp>
#include <Model3D.hpp>

#include <Window.hpp>
#include <Input.hpp>

#include <SoundWav.hpp>
#include <Sound.hpp>

#include <AssetLoader.hpp>
#include <AssetManager.hpp>

#include "EnginePipeline.hpp"
#include "EngineWindow.hpp"
#include "World.hpp"

using namespace Tudo;

class Engine : public Program
{
public:
	Engine();
	void OnResize(vec2i& size);

protected:
	bool Initialize();
	void Tick();
	void Draw();
	void Cleanup();

private:
	void RegisterActors();

private:
	Clock			mClock;
	Input			mInput;

	GraphicsDevice* mGDevice;
	EnginePipeline* mPipeline;
	EngineWindow*	mWindow;

	SoundManager*	mSoundManager;
	AssetLoader*	mAssetLoader;

	World*			mWorld;
};
#endif