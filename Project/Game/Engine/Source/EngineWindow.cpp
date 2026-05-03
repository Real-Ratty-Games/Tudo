/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "EngineWindow.hpp"
#include "Engine.hpp"

EngineWindow::EngineWindow(Program* program)
{
	pProgram = program;
}

void EngineWindow::EventCallback()
{
	switch (mWndEvent.type)
	{
	case SDL_EVENT_QUIT: pProgram->Quit(); break;
	case SDL_EVENT_WINDOW_MINIMIZED: bIconified = true; break;
	case SDL_EVENT_WINDOW_RESTORED: bIconified = false; break;
	case SDL_EVENT_WINDOW_RESIZED:
	{
		vec2i size = GetSize();
		dynamic_cast<Engine*>(pProgram)->OnResize(size);
	}break;
	}
}
