/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "Engine.hpp"
#include <FileSystem.hpp>
#include <Logger.hpp>
#include <IniFile.hpp>

#include "StaticModel.hpp"

Engine::Engine() : Program(),
	mClock(120)
{ }

void Engine::OnResize(vec2i& size)
{
	mGDevice->OnResize(size, false);
	mPipeline->OnResize(size);
}

bool Engine::Initialize()
{
	Window::Initialize();
	Window::ShowSplashScreen();

	mSoundManager = new SoundManager();

	vec2 res = vec2(800, 600);

	mWindow = new EngineWindow(this);
	mWindow->Create("Tudo Engine Game", res.X, res.Y, true, false);

	mGDevice = new GraphicsDevice(*mWindow, EDrawAPI::DIRECT3D11, false);

	mAssetLoader = new AssetLoader(*mGDevice, "Data");

	mPipeline = new EnginePipeline(*mGDevice, *mAssetLoader, res);

	mWorld = new World();
	RegisterActors();

	Window::DestroySplashScreen();
	mWindow->Show();
	return true;
}

void Engine::Tick()
{
	mWindow->PollEvent();

	if (mInput.KeyboardKeyPressed(KeyboardKey::F1))
		mWindow->SwitchFullscreen();

	if (mInput.KeyboardKeyPressed(KeyboardKey::ESCAPE))
		Quit();

	mWorld->PreTick();

	mClock.Tick();
	while (mClock.Wait())
		mWorld->Tick();
}

void Engine::Draw()
{
	mGDevice->BeginDraw();
	if (!mWindow->IsIconified())
		mPipeline->DrawAll();
	mGDevice->EndDraw();
}

void Engine::Cleanup()
{
	TUDO_MEM_FREE(mWorld);
	TUDO_MEM_FREE(mAssetLoader);
	TUDO_MEM_FREE(mSoundManager);
	TUDO_MEM_FREE(mPipeline);
	TUDO_MEM_FREE(mGDevice);
	TUDO_MEM_FREE(mWindow);

	Window::Release();
}

void Engine::RegisterActors()
{
	mWorld->GetActorFactory().Register<StaticModel>("StaticModel");
}
