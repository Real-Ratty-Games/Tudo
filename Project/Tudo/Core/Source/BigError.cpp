/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "BigError.hpp"
#include "Window.hpp"

#if _WIN32 && _DEBUG
#include <Windows.h>
#include <debugapi.h>
#endif

using namespace Tudo;

BigError::BigError(const strg& message) : std::runtime_error(message)
{
	if (Window::IsSDLInit())
	{
		if (Window::IsSplashScreenVisible())
			Window::DestroySplashScreen();
#if !_DEBUG
		Window::ShowMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error", message);
#endif
	}

#if _DEBUG
	strg dstr = "\n\n##########################################\n\n            Tudo Engine Error\n\n" + message + "\n##########################################\n\n";
	OutputDebugString(TEXT(dstr.c_str()));
#endif
}
