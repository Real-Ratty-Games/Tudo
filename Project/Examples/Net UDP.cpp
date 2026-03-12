/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "../Include/Game.hpp"
#include "../Include/GameWindow.hpp"
#include <FileSystem.hpp>
#include <Renderer.hpp>
#include <Input.hpp>

#include <Network.hpp>
#include <NetServerUDP.hpp>
#include <NetClientUDP.hpp>

using namespace MyGame;

class MyServer : public NetServerUDP
{
protected:
	int ListenToClient()
	{
		NetClientDataUDP client;

		char buffer[14];
		const int bytes = Recv(client, buffer, sizeof(buffer));

		if (bytes != GAMEENGINE_NET_NOTHING)
		{
			buffer[bytes - 1] = '\0';
			const strg header = client.IP;
			Window::ShowMessageBox(SDL_MESSAGEBOX_INFORMATION, header, buffer);
		}
		return bytes;
	}
};

class MyClient : public NetClientUDP
{
public:
	int ListenToServer()
	{
		if (Keyboard::KeyPressed(KeyboardKey::SPACE))
		{
			const char* buffer = "Hello Server! ";
			Send(buffer, strlen(buffer));
		}

		return GAMEENGINE_NET_NOTHING;
	}
};

void GameProgram::OnResize(vec2i& size)
{
	// EMPTY
}

static MyServer _inst;
// static MyClient _inst;

bool GameProgram::Initialize()
{
	// create window
	Window::Initialize();

	const vec2 resolution(1280, 720);

	mWindow = new GameWindow(this);
	mWindow->Create("Server", (uint)resolution.X, (uint)resolution.Y, false);

	Network::Initialize();
	_inst.Initialize(54000, "0.0.0.0");

	mWindow->Show();
	return true;
}

void GameProgram::Tick()
{
	_inst.Run();
}

void GameProgram::Cleanup()
{
	if (mWindow != nullptr)
	{
		delete mWindow;
		mWindow = nullptr;
	}
	Window::Release();

	_inst.Release();
	Network::Release();
}
