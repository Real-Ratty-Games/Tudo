/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "Window.hpp"
#include "FileSystem.hpp"
#include "Logger.hpp"
#include "WindowCursor.hpp"

using namespace Tudo;

SDL_Window*		Window::sSplashWndHandle	= nullptr;
SDL_Renderer*	Window::sSplashWndRenderer	= nullptr;
bool			Window::sbSplashWndVisible	= false;
bool			Window::sbSDLInit			= false;

void Window::Initialize()
{
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
		Logger::Log("Window::Initialize", "Failed initializing SDL3: " + strg(SDL_GetError()), ELogType::LERROR);
	sbSDLInit = true;
	Logger::Log("SDL3 successfully initialized!");
}

void Window::Release()
{
	sbSDLInit = false;
	SDL_Quit();
	Logger::Log("SDL3 released!");
}

void Window::ShowSplashScreen(strgv filename)
{
	const strg filePath = FileSystem::GetResourcePath(filename).string();
	if (!FileSystem::Exists(filePath))
		Logger::Log("Window::ShowSplashScreen", "File does not exist: " + strg(filename), ELogType::LERROR);

	SDL_Surface* surface = SDL_LoadBMP(filePath.data());

	sSplashWndHandle = SDL_CreateWindow("Splash", surface->w, surface->h, SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP);
	sSplashWndRenderer = SDL_CreateRenderer(sSplashWndHandle, NULL);
	SDL_SetRenderDrawColor(sSplashWndRenderer, 0, 0, 0, 255);
	SDL_RenderClear(sSplashWndRenderer);

	if (surface != nullptr)
	{
		SDL_BlitSurface(surface, NULL, SDL_GetWindowSurface(sSplashWndHandle), NULL);

		SDL_Texture* splashTexture = SDL_CreateTextureFromSurface(sSplashWndRenderer, surface);
		SDL_DestroySurface(surface);

		SDL_RenderTexture(sSplashWndRenderer, splashTexture, NULL, NULL);
	}
	SDL_RenderPresent(sSplashWndRenderer);

	sbSplashWndVisible = true;
}

void Window::SetHardwareCursorImage(WindowCursor* cursor)
{
	SDL_Cursor* cur = (cursor == nullptr) ? nullptr : cursor->mCursor;
	SDL_SetCursor(cur);
}

void Window::SetCursor(SDL_SystemCursor id)
{
	SDL_Cursor* cur = SDL_CreateSystemCursor(id);
	SDL_SetCursor(cur);
}

void Window::ShowMessageBox(SDL_MessageBoxFlags flags, strgv header, strgv message, Window* window)
{
	SDL_ShowSimpleMessageBox(flags, header.data(), message.data(), (window == nullptr) ? nullptr : window->mWndHandle);
}

void Window::DestroySplashScreen()
{
	SDL_DestroyRenderer(sSplashWndRenderer);
	SDL_DestroyWindow(sSplashWndHandle);
	sbSplashWndVisible = false;
}

bool Window::IsSplashScreenVisible()
{
	return sbSplashWndVisible;
}

bool Window::IsSDLInit()
{
	return sbSDLInit;
}

void Window::Create(strgv title, uint width, uint height, bool resizable, bool fs)
{
	bFullscreen = fs;

    SDL_WindowFlags wflags = ((fs ? SDL_WINDOW_FULLSCREEN : 0) | SDL_WINDOW_HIDDEN);
#if __APPLE__
    wflags |= SDL_WINDOW_METAL;
#endif
    
	if (resizable) wflags |= SDL_WINDOW_RESIZABLE;

	if ((mWndHandle = SDL_CreateWindow(title.data(), (int)width, (int)height, wflags)) == nullptr)
		Logger::Log("Window::Create", "Failed creating window: " + strg(SDL_GetError()), ELogType::LERROR);
    
#if __APPLE__
    SDL_Renderer* mrenderer = SDL_CreateRenderer(mWndHandle, "Metal");
    if (!mrenderer)
		Logger::Log("Window::Create", "Failed creating SDL renderer: " + strg(SDL_GetError()), ELogType::LERROR);
#endif

	Logger::Log("New Window created!");
}

void Window::Show(bool vl)
{
	if (vl) SDL_ShowWindow(mWndHandle);
	else SDL_HideWindow(mWndHandle);
}

void Window::PollEvent()
{
	while (SDL_PollEvent(&mWndEvent)) EventCallback();
}

void Window::Destroy()
{
	SDL_DestroyWindow(mWndHandle);
}

void Window::WarpMouseInWindow(float x, float y)
{
	SDL_WarpMouseInWindow(mWndHandle, x, y);
}

bool Window::IsIconified()
{
	return bIconified;
}

void Window::SetSize(uint width, uint height)
{
	SDL_SetWindowSize(mWndHandle, (int)width, (int)height);
}

void Window::Center()
{
	SDL_SetWindowPosition(mWndHandle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

void Window::SetFullscreen(bool vl)
{
	bFullscreen = vl;
	SDL_SetWindowFullscreen(mWndHandle, vl);
}

void Window::SwitchFullscreen()
{
	SetFullscreen(!bFullscreen);
}
 
vec2i Window::GetSize()
{
	int w, h;
	SDL_GetWindowSize(mWndHandle, &w, &h);
	return vec2i(w, h);
}

bool Window::GetFullscreen()
{
	return bFullscreen;
}

void* Window::GetNativePtr()
{
	SDL_PropertiesID wid = SDL_GetWindowProperties(mWndHandle);
#if WIN32
	void* hwnd = SDL_GetPointerProperty(wid, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);
	return hwnd;
#elif __APPLE__
    SDL_Renderer* mrenderer = SDL_GetRenderer(mWndHandle);
    return SDL_GetRenderMetalLayer(mrenderer);
#endif
	Logger::Log("Window::GetNativePtr", "Invalid Platform!", ELogType::LERROR);
}
