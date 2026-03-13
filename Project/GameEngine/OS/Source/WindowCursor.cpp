/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "WindowCursor.hpp"
#include "BigError.hpp"

using namespace GameEngine;

void WindowCursor::Load(strgv imagePath)
{
	SDL_Surface* cursorSurface = SDL_LoadBMP(imagePath.data());
	if (cursorSurface != nullptr)
	{
		mCursor = SDL_CreateColorCursor(cursorSurface, 0, 0);
		SDL_DestroySurface(cursorSurface);
	}
	else throw BigError("Image file not found: " + strg(imagePath));
}

WindowCursor::~WindowCursor()
{
	SDL_DestroyCursor(mCursor);
}
