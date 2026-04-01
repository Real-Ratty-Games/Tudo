/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "../Include/Game.hpp"
#include "../Include/GameWindow.hpp"
#include "../Include/GamePipeline.hpp"

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


	Gamepad::LoadConfig();


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



	int moveUD = 0;
	int moveLR = 0;

	if (mGamepad.IsConnected())
	{
		if (mGamepad.ButtonDown(GamepadButton::DPAD_DOWN))
			moveUD = 1;
		else if (mGamepad.ButtonDown(GamepadButton::DPAD_UP))
			moveUD = -1;

		if (mGamepad.ButtonDown(GamepadButton::DPAD_RIGHT))
			moveLR = 1;
		else if (mGamepad.ButtonDown(GamepadButton::DPAD_LEFT))
			moveLR = -1;
	}
	else mGamepad.Connect(0);

	mClock.Tick();
	while (mClock.Wait())
	{
		if (moveUD != 0)
		{
			Transform2D& transf = mPipeline->QuadTransf;
			float last = transf.Location.Y;
			transf.Location.Y += 10.0f * moveUD;

			if (transf.Location.Y <= 0 || transf.Location.Y >= 720)
				transf.Location.Y = last;
		}

		if (moveLR != 0)
		{
			Transform2D& transf = mPipeline->QuadTransf;
			float last = transf.Location.X;
			transf.Location.X += 10.0f * moveLR;

			if (transf.Location.X <= 0 || transf.Location.X >= 1280)
				transf.Location.X = last;
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
