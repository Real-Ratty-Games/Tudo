/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "../Include/Game.hpp"
#include "../Include/GameWindow.hpp"
#include "../Include/GamePipeline.hpp"
#include <Multiply.hpp>

using namespace MyGame;

void GameProgram::OnResize(vec2i& size)
{
	mGDevice->OnResize(size, true);
	mPipeline->OnResize(size);
}

bool GameProgram::Initialize()
{
	// create window
	Window::Initialize();
	Window::ShowSplashScreen();

	mSound = new SoundManager();

	const vec2 resolution(1280, 720);

	mWindow = new GameWindow(this);
	mWindow->Create("Tudo Engine Game", (uint)resolution.X, (uint)resolution.Y, false);

	// init renderer
	DrawAPI dapi = DrawAPI::DIRECT3D11;
#if __APPLE__
	dapi = DrawAPI::METAL;
#endif

	mGDevice = new GraphicsDevice(*mWindow, dapi, true);

	mAssetLoader = new AssetLoader(*mGDevice, "Data");

	mPipeline = new GamePipeline(*mGDevice, *mAssetLoader, resolution);

	// set hw cursor
	mAssetLoader->LoadHardwareCursorImage(mCursor, "Cursor.bmp");
	mWindow->SetHardwareCursorImage(&mCursor);

	Window::DestroySplashScreen();
	mWindow->Show();
	return true;
}

void GameProgram::Tick()
{
	mWindow->PollEvent();

	if (mInput.KeyboardKeyPressed(KeyboardKey::N1))
		mWindow->SwitchFullscreen();

	if (mInput.KeyboardKeyPressed(KeyboardKey::ESCAPE))
		Quit();

	mClock.Tick();
	while (mClock.Wait())
	{
		Viewport3D& vp = mPipeline->_vp3d;

		if (mInput.KeyboardKeyDown(KeyboardKey::LEFT))
		{
			vec3 rotationAxis = vp.Up();
			quat q = Math::QuatFromAxisAngle(rotationAxis, Math::ToRadians(5.0f));
			vec3 eyeOffset = vp.Eye;
			vec3 rotatedOffset = Math::Multiply(eyeOffset, q);
			vp.Eye = vp.Target + rotatedOffset;

			vp.CreateView();
		}
		else if (mInput.KeyboardKeyDown(KeyboardKey::RIGHT))
		{
			vec3 rotationAxis = -vp.Up();
			quat q = Math::QuatFromAxisAngle(rotationAxis, Math::ToRadians(5.0f));
			vec3 eyeOffset = vp.Eye;
			vec3 rotatedOffset = Math::Multiply(eyeOffset, q);
			vp.Eye = vp.Target + rotatedOffset;

			vp.CreateView();
		}
	}
}

void GameProgram::Draw()
{
	mGDevice->BeginDraw();
	if (!mWindow->IsIconified())
		mPipeline->DrawAll();
	mGDevice->EndDraw();
}

void GameProgram::Cleanup()
{
	Window::SetHardwareCursorImage(nullptr);
	Window::Release();
}
