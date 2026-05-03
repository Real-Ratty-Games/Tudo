/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "WindowCursor.hpp"
#include "Logger.hpp"

using namespace Tudo;

WindowCursor::~WindowCursor()
{
	SDL_DestroyCursor(mCursor);
	Logger::Log("WindowCursor '" + mName + "' released!");
}

strg WindowCursor::Name()
{
	return mName;
}
